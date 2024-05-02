// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"


// Forward declarations for Enemy class
class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class UPawnSensingComponent;
class AAIController;

/**
 * Enemy class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Function that handles which direction the enemy moves when getting hit
	void DirectionalHitReact(const FVector& ImpactPoint) const;

	// Function that handles enemy reaction when getting hit
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	// Function that makes enemy take damage when getting hit
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void ToggleHealthBarWidget(bool Toggle) const;
	
	// Function called when the enemy dies
	void Die();

	bool InTargetRange(const AActor* Target, double Radius) const;

	void MoveToTarget(const AActor* Target) const;

	AActor* ChoosePatrolTarget();

	void CheckPatrolTarget();
	
	void CheckCombatTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	
	/**
	 *	Hit react montage functions
	 */
	void PlayHitReactMontage(const FName& SectionName) const;

	// Initialise enemy death pose to Alive
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	
private:

	// Variable storing which actor is the target for combat
	UPROPERTY()
	AActor* CombatTarget;

	// Distance from character that the enemy requires to start combat 
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	// Variable storing attribute component
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	// Variable storing health bar widget
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;
	
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

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	/**
	 *	Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;
	
	/**
	 *	Sound and particle variables when getting hit
	 */
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	UParticleSystem* HitParticles;
};
