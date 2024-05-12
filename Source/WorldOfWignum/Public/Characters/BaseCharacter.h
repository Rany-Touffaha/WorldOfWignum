// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
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

protected:
	virtual void BeginPlay() override;

	/** Combat */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Attack();
	virtual void Die();
	void DirectionalHitReact(const FVector& ImpactPoint) const;
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint) const;
	void SpawnHitParticles(const FVector& ImpactPoint) const;
	void DisableCapsule() const;
	virtual bool CanAttack() const;
	bool IsAlive() const;
	void DisableMeshCollision() const;

	/** Montage */
	void PlayHitReactMontage(const FName& SectionName) const;
	virtual int32 PlayAttackMontage() const;
	virtual int32 PlayDeathMontage();
	void StopAttackMontage() const;

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget() const;

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget() const;
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled) const;
	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;
	
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;
	
private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName) const;
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames) const;
	
	/**
	 *	Sound and particle variables when getting hit
	 */
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;

	/**
	*	Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> DeathMontageSections;

public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
};
