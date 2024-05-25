// World of Wignum by Rany Touffaha

#include "Characters/BaseCharacter.h"

#include "Characters/CharacterTypes.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create attribute component
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// Set up collision response for capsule
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

/**
* Handles base character's reaction when getting hit using IHitInterface 
* @param ImpactPoint Location of impact point of the hit
* @param Hitter Actor that is hitting the base enemy
*/
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	IsAlive() && Hitter ? DirectionalHitReact(Hitter->GetActorLocation()) : Die();
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

/**
 * Attacks combat target if valid. Resets combat target after attack.
 */
void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
		CombatTarget = nullptr;
}

/**
 * Determines the direction of where the weapon 
 * @param ImpactPoint Location of the weapon's impact point
 */
void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint) const
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
		Theta *= -1.f;

	// Determine which quadrant based on the angle theta
	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
		
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
		
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);
}

/**
 * Updates character's health in the attributes component
 * @param DamageAmount Amount of damage to be received
 */
void ABaseCharacter::HandleDamage(const float DamageAmount)
{
	if (Attributes)
		Attributes->ReceiveDamage(DamageAmount);
}

/**
 * Plays hit sound at the weapon's impact point when getting hit
 * @param ImpactPoint Location of the weapon's impact point
 */
void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint) const
{
	if(HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
}

/**
 * Spawns hit particles at the weapon's impact point when getting hit
 * @param ImpactPoint Location of the weapon's impact point
 */
void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint) const
{
	if(HitParticles && GetWorld())
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles,ImpactPoint);
}

/**
 * Disables capsule component
 */
void ABaseCharacter::DisableCapsule() const
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/**
 * Checks if base character can attack.
 * @return false for base class, to be overridden in children classes.
 */
bool ABaseCharacter::CanAttack() const
{
	return false;
}

/**
 * Check if base character is alive
 * @return true if base character is alive, false otherwise
 */
bool ABaseCharacter::IsAlive() const
{
	return Attributes && Attributes->IsAlive();
}

/**
 * Disable base character's mesh collision
 */
void ABaseCharacter::DisableMeshCollision() const
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/**
 * Plays the HitReact animation section in the animation montage
 * @param SectionName Name of section in the HitReact animation montage to be played
 */
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName) const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

/**
 *  Plays a random section in the attack animation montage
 * @return a random section name from the list of montage sections in the attack montage
 */
int32 ABaseCharacter::PlayAttackMontage() const
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

/**
 * Plays a random section in the death animation montage
 * @return a random section name from the list of montage sections in the death montage
 */
int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	if(const TEnumAsByte<EDeathPose> Pose(Selection); Pose < EDP_MAX)
		DeathPose = Pose;

	return Selection;
}

/**
 * Plays the dodge animation montage
 */
void ABaseCharacter::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}

/**
 * Stops attack montage if base character gets hit
 */
void ABaseCharacter::StopAttackMontage() const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
}


/**
 * Initiates base character's death.
 */
void ABaseCharacter::Die_Implementation()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}

/**
 * Determines distance needed to warp to the combat target. Called within the animation montage.
 * @return Location of combat target to warp to
 */
FVector ABaseCharacter::GetTranslationWarpTarget() const
{
	if (CombatTarget == nullptr)
		return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;
}

/**
 * Determines rotation needed to warp to the combat target. Called within the animation montage.
 * @return Rotation of combat target to warp to
 */
FVector ABaseCharacter::GetRotationWarpTarget() const
{
	return CombatTarget ? CombatTarget->GetActorLocation() : FVector();
}

/**
 * Determines the end of the attack. To be called in the animation montage.
 */
void ABaseCharacter::AttackEnd(){}

/**
 * Determines the end of the dodge. To be called in the animation montage.
 */
void ABaseCharacter::DodgeEnd(){}

/**
 * Enables weapon collision. To be called in blueprints. 
 * @param CollisionEnabled Type that enables or disables collision of the weapon
 */
void ABaseCharacter::SetWeaponCollisionEnabled(const ECollisionEnabled::Type CollisionEnabled) const
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

/**
 * Plays a section of a specific animation montage
 * @param Montage Name of the animation montage to be played
 * @param SectionName Name of the montage section to be played
 */
void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName) const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

/**
 * Plays a random section in a specific animation montage
 * @param Montage Name of the animation montage to be played
 * @param SectionNames List of montage sections to be played
 * @return a random section name from the list of montage section names
 */
int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames) const
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}










