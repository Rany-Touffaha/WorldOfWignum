// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Kwang.generated.h"

// Forward declarations for Kwang class
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;

/**
 * Kwang character class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API AKwang : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AKwang();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	/** Callback functions for input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	virtual void Attack() override;
	void Dodge();

	/** Combat */
	void EquipWeapon(AWeapon* Weapon);
	virtual bool CanAttack() const override;
	virtual void AttackEnd() override;
	bool CanDisarm() const;
	bool CanArm() const;
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName) const;

		
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	
	/**
	 *	Input Actions
	 */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* KwangContext;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;

private:
	/** Character Components */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
 