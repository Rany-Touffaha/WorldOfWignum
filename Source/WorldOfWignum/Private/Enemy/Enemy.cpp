// World of Wignum by Rany Touffaha

#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Soul.h"
#include "Items/Weapons/Weapon.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set collision types for different components in the enemy class
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	// Set up the view health bar widget
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// Make enemy rotate towards our movement
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Remove inheritance of Enemy's class rotation from the Controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create and set up pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	EnemyState > EEnemyState::EES_Patrolling ? CheckCombatTarget() : CheckPatrolTarget();
}

/**
 * Takes damage from Kwang's health
 * @param DamageAmount Amount of damage to be taken
 * @param DamageEvent Event type used by UE
 * @param EventInstigator Controller that instigates the event
 * @param DamageCauser Actor that causes the damage
 * @return Amount of damage to be taken from Kwang's health
 */
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		ChaseTarget();
	}
	return DamageAmount;
}

/**
 * Destroys equipped weapon
 */
void AEnemy::Destroyed()
{
	if(EquippedWeapon)
		EquippedWeapon->Destroy();
}

/**
 * Handles Kwang's reaction when getting hit using IHitInterface 
 * @param ImpactPoint Location of impact point of the hit
 * @param Hitter Actor that is hitting Kwang
 */
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsDead())
		ShowHealthBar();
	
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	StopAttackMontage();
	
	if (IsInsideAttackRadius() && !IsDead())
		StartAttackTimer();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if(PawnSensing)
			PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	
	InitialiseEnemy();
	Tags.Add(FName("Enemy"));
}

/**
 * Spawn a swoul at the enemy's location
 */
void AEnemy::SpawnSoul()
{
	if (UWorld* World = GetWorld(); World && SoulClass && Attributes)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		if (ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation()))
		{
			SpawnedSoul->SetSouls(Attributes->GetSouls());
			SpawnedSoul->SetOwner(this);
		}
	}
}

/**
 * Initiates enemy's death.
 */
void AEnemy::Die_Implementation()
{
	Super::Die_Implementation();
	
	EnemyState = EEnemyState::EES_Dead;
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnSoul();
}

/**
 * Initiates enemy attack
 */
void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

/**
 * Checks if enemy can attack
 * @return true if the enemy can attack, false otherwise
 */
bool AEnemy::CanAttack() const
{
	const bool bCanAttack =
		IsInsideAttackRadius() &&
			!IsAttacking() &&
				!IsEngaged() &&
					!IsDead();
	
	return bCanAttack;
}

/**
 * Changes enemy state and combat target when the attack has ended 
 */
void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

/**
 * Updates character's health in the attributes component
 * @param DamageAmount Amount of damage to be received
 */
void AEnemy::HandleDamage(const float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

/**
 * Initialise enemy movement and functionality
 */
void AEnemy::InitialiseEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}

/**
 * Checks patrol targets
 */
void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

/**
 * Checks combat targets
 */
void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged())
			StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged())
			ChaseTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

/**
 * Moves to next target once patrol timer is finished 
 */
void AEnemy::PatrolTimerFinished() const
{
	MoveToTarget(PatrolTarget);	
}

/**
 * Hides health bar
 */
void AEnemy::HideHealthBar() const
{
	if (HealthBarWidget)
		HealthBarWidget->SetVisibility(false);
}

/**
 * Shows health bar 
 */
void AEnemy::ShowHealthBar() const
{
	if (HealthBarWidget)
		HealthBarWidget->SetVisibility(true);
}

/**
 * Makes enemy lose interest
 */
void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

/**
 * Starts enemy patrolling
 */
void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

/**
 * Chases target
 */
void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

/**
 * Checks if target is outside combat radius
 * @return true if outside combat radius, false otherwise
 */
bool AEnemy::IsOutsideCombatRadius() const
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

/**
 * Checks if target is outside attack radius
 * @return true if outside attack radius, false otherwise
 */
bool AEnemy::IsOutsideAttackRadius() const
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

/**
 * Checks if target is inside attack radius
 * @return true if inside attack radius, false otherwise
 */
bool AEnemy::IsInsideAttackRadius() const
{
	return InTargetRange(CombatTarget, AttackRadius);
}

/**
 * Checks if enemy state is in chasing mode
 * @return true if enemy state is set to Chasing, false otherwise
 */
bool AEnemy::IsChasing() const
{
	return EnemyState == EEnemyState::EES_Chasing;
}

/**
 * Checks if enemy state is in attacking mode
 * @return true if enemy state is set to Attacking, false otherwise
 */
bool AEnemy::IsAttacking() const
{
	return EnemyState == EEnemyState::EES_Attacking;
}

/**
 * Checks if enemy state is in dead state
 * @return true if enemy state is set to Dead, false otherwise
 */
bool AEnemy::IsDead() const
{
	return EnemyState == EEnemyState::EES_Dead;
}

/**
 * Checks if enemy state is in Engaged state
 * @return true if enemy state is set to Engaged, false otherwise
 */
bool AEnemy::IsEngaged() const
{
	return EnemyState == EEnemyState::EES_Engaged;
}

/**
 * Clears patrol timer
 */
void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);	
}

/**
 * Starts attack timer
 */
void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

/**
 * Clears attack timer
 */
void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

/**
 * Checks if target is in range
 * @param Target Actor that is set as the target
 * @param Radius Distance between the target and the current enemy
 * @return true if target is in range, false otherwise
 */
bool AEnemy::InTargetRange(const AActor* Target, const double Radius) const
{
	if (Target == nullptr) return false;
	
	// Calculate the distance between enenmy and target actor
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

/**
 * Makes enemy move to the set target
 * @param Target Actor that is set as the target
 */
void AEnemy::MoveToTarget(const AActor* Target) const
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	EnemyController->MoveTo(MoveRequest);
}

/**
 * Randomly select a patrol target
 * @return Actor that is set as the patrol target
 */
AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if(Target != PatrolTarget)
			ValidTargets.AddUnique(Target);
	}

	if (const int32 NumPatrolTargets = ValidTargets.Num(); NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets -1);
		return ValidTargets[TargetSelection];
	}
	
	return nullptr;
}

/**
 * Spawns a default weapon for the enemy
 */
void AEnemy::SpawnDefaultWeapon()
{
	if(UWorld* World = GetWorld(); World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

/**
 * Callback function for OnPawnSeen in UPawnSensingComponent
 * @param SeenPawn Pawn that has been see and needs to be set at the combat target
 */
void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
			EnemyState != EEnemyState::EES_Chasing &&
				EnemyState < EEnemyState::EES_Attacking &&
					SeenPawn->ActorHasTag(FName("EngageableTarget"));
					
	if(bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}