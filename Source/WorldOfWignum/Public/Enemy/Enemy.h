// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"


// Forward declarations for Enemy class
class UHealthBarComponent;
class UPawnSensingComponent;
class AAIController;
class AWeapon;

/**
 * Enemy class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	
	// Function that handles enemy reaction when getting hit
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	// Function that makes enemy take damage when getting hit
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	
	// Function called when the enemy dies
	virtual void Die() override;

	bool InTargetRange(const AActor* Target, double Radius) const;

	void MoveToTarget(const AActor* Target) const;

	AActor* ChoosePatrolTarget();

	void CheckPatrolTarget();
	
	void CheckCombatTarget();

	virtual void Attack() override;
	virtual bool CanAttack() const override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.f;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	
	// Initialise enemy death pose to Alive
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
private:

	// Variable storing which actor is the target for combat
	UPROPERTY()
	AActor* CombatTarget;

	// Distance from character that the enemy requires to start chasing 
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	// Distance from character that the enemy requires to start attacking
	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;
	
	// Variable storing health bar widget
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;
	
	/**
	 * Navigation
	 */

	UPROPERTY()
	AAIController* EnemyController;
	
	// Current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	// Distance from target that the enemy requires to patrol 
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	void PatrolTimerFinished() const;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;


	/** AI Behaviour */

	void HideHealthBar() const;
	void ShowHealthBar() const;
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius() const;
	bool IsOutsideAttackRadius() const;
	bool IsInsideAttackRadius() const;
	bool IsChasing() const;
	bool IsAttacking() const;
	bool IsDead() const;
	bool IsEngaged() const;
	void ClearPatrolTimer();

	/** Combat */
	void StartAttackTimer();
	void ClearAttackTimer();
	
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;	

	
};

