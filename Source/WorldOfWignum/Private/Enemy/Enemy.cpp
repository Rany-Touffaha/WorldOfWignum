// World of Wignum by Rany Touffaha

#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"

/**
 * Enemy class constructor
 */
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set collision types for different components in the enemy class
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Set up the attribute compnent 
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// Set up the view health bar widget
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Initially hide enemy health bar
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

// Function called when the enemy dies
void AEnemy::Die()
{
	// Plays death montages randomly
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0,5);
		FName SectionName;
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5:
			SectionName = FName("Death6");
			DeathPose = EDeathPose::EDP_Death6;
			break;
		default:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		}
		
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	// Hides enemy health bar when dead
	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	// Disable enemy capsule when dead
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Remove enemy from world when dead
	SetLifeSpan(3.f);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		// Calculate the distance between enenmy and character
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();

		// Disable health bar and remove combat target if the character is too far
		if (DistanceToTarget > CombatRadius)
		{
			CombatTarget = nullptr;
			if(HealthBarWidget)
			{
				HealthBarWidget->SetVisibility(false);
			}
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Function that plays the hit react montages
void AEnemy::PlayHitReactMontage(const FName& SectionName) const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

// Function that handles which direction the enemy moves when getting hit
void AEnemy::DirectionalHitReact(const FVector& ImpactPoint) const
{
	// Calculate the forward and hit vectors 
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Calculate the dot product of the forward and hit vectors 
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// Determine the orientation of the angle based on the cross product
	if (const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit); CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	// Determine which quadrant based on the angle theta
	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
		
	} else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
		
	} else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);
}

// Function that handles enemy reaction when getting hit
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	// Enable enemy health bar when getting hit
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}

	// Enemy gets hit by weapon, dies otherwise
	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
	
	// Plays sound if gets hit
	if(HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	// Spawn blood particles effect when getting hit
	if(HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

// Function that makes enemy take damage when getting hit
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);

		// Update health bar widget after getting hit
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}

	// Set the combat target when getting attacked
	CombatTarget = EventInstigator->GetPawn();
	
	return DamageAmount;
}

