// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
class AWeapon;

/**
 * Kwang character class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API AKwang : public ACharacter
{
	GENERATED_BODY()

public:
	AKwang();
	virtual void Tick(float DeltaTime) override;

	// Override SetupPlayerInputComponent function to bind input actions to corresponding functions
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Set weapon collision depending on which momenent the character is attacking
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled) const;

protected:
	virtual void BeginPlay() override;

	// Input Mapping Context for Kwang character
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* KwangContext;
	
	/**
	 *	Input Actions
	 */
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

	/**
	 *	Callback functions for input
	 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void Attack();
	void Dodge();

	/**
	 *	Play attack montage functions
	 */
	void PlayAttackMontage() const;
	bool CanAttack() const;

	/**
	 *	Play equip montage functions
	 */
	void PlayEquipMontage(const FName& SectionName) const;
	bool CanDisarm() const;
	bool CanArm() const;
	
	// Function to change action state to unoccupied at the end of an attack
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	// Function to attach the weapon to the back of the character
	UFUNCTION(BlueprintCallable)
	void Disarm();

	// Function to attach the weapon to the right hand of the character
	UFUNCTION(BlueprintCallable)
	void Arm();

	// Function to reset the state to unoccupied when done equipping
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

private:
	// Initialise character state to Unequipped
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	// Initialise action state to Unoccupied
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	// Spring arm component for camera control
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	// Camera component for character's view
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	// Variable storing current overlapping item
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	// Variable storing current equipped weapon
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	/**
	 *	Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;
	
public:
	// Function to set overlapping item
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

	// Function to get character state
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
 