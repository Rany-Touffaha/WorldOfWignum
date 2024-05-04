// World of Wignum by Rany Touffaha

#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "WorldOfWignum/DebugMacros.h"

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
	
	// Set up the view health bar widget
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// Make enemy rotate towards our movement
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Remove inheritance of Enemy's class rotation from the Controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Initially hide enemy health bar
	ToggleHealthBarWidget(false);
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	
	if(PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	if(UWorld* World = GetWorld(); World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

void AEnemy::ToggleHealthBarWidget(const bool Toggle) const
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(Toggle);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		// Outside combat radius, lose interest
		CombatTarget = nullptr;
		ToggleHealthBarWidget(false);
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		// Outside attack range, chase character
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		// Inside attack range, attack character
		EnemyState = EEnemyState::EES_Attacking;
		// TODO: Add attack montage
		Attack();
	}
}

void AEnemy::Attack()
{
	Super::Attack();
	PlayAttackMontage();
}

void AEnemy::PlayAttackMontage() const
{
	Super::PlayAttackMontage();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0,1);
		FName SectionName;
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			SectionName = FName("Attack1");
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if(EnemyState == EEnemyState::EES_Chasing) return;
	if (SeenPawn->ActorHasTag(FName("Kwang")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;
		
		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
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
	ToggleHealthBarWidget(false);
	
	// Disable enemy capsule when dead
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Remove enemy from world when dead
	SetLifeSpan(3.f);
}

bool AEnemy::InTargetRange(const AActor* Target, const double Radius) const
{
	if (Target == nullptr) return false;
	
	// Calculate the distance between enenmy and target actor
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(const AActor* Target) const
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if(Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
			
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets -1);
		return ValidTargets[TargetSelection];
	}
	
	return nullptr;
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::PatrolTimerFinished() const
{
	MoveToTarget(PatrolTarget);	
}

// Function that handles enemy reaction when getting hit
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	// Enable enemy health bar when getting hit
	ToggleHealthBarWidget(true);

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
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MoveToTarget(CombatTarget);
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

