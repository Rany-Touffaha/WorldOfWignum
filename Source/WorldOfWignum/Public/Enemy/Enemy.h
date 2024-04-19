// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;

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
	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	/**
	 *	Hit react montage functions
	 */
	void PlayHitReactMontage(const FName& SectionName) const;
	
private:

	/**
	 *	Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	/**
	 *	Sound and particle variables when getting hit
	 */
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	UParticleSystem* HitParticles;
};
