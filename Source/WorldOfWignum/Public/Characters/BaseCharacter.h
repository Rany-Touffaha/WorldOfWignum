// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class UAttributeComponent;
class AWeapon;
class UAnimMontage;

UCLASS()
class WORLDOFWIGNUM_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	// Set weapon collision depending on which momenent the character is attacking
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled) const;
	
protected:
	virtual void BeginPlay() override;
	virtual void Attack();
	// Function called when the enemy dies
	virtual void Die();

	/**
	 *	Play attack montage functions
	 */
	virtual void PlayAttackMontage() const;
	virtual bool CanAttack() const;

	/**
	*	Hit react montage functions
	*/
	void PlayHitReactMontage(const FName& SectionName) const;

	// Function that handles which direction the enemy moves when getting hit
	void DirectionalHitReact(const FVector& ImpactPoint) const;

	// Function to change action state to unoccupied at the end of an attack
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	// Variable storing current equipped weapon
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	/**
	 *	Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	// Variable storing attribute component
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	/**
	 *	Sound and particle variables when getting hit
	 */
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	UParticleSystem* HitParticles;
};
