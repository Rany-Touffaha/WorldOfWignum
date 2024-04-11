#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Kwang.generated.h"

//Forward declarations for Kwang class
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;

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

protected:
	virtual void BeginPlay() override;

	// Input Mapping Context for Kwang character
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* KwangContext;

	// Movement action input
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	// Look action input
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	// Jump action input
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	// E key action input
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyAction;

	// Attack action input
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	// Dodge action input
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;

	// Function to handle movement input
	void Move(const FInputActionValue& Value);

	// Function to handle looking around input
	void Look(const FInputActionValue& Value);

	// Function to handle E key action
	void EKeyPressed();

	// Function to handle attack action
	void Attack();

	// Function to handle dodge action
	void Dodge();
	
private:
	// Initialise character state to Unequipped
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	// Spring arm component for camera control
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	// Camera component for character's view
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	// Variable storing current overlapping item
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	
public:
	// Function to set overlapping item
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

	// Function to get character state
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
 