// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "KwangAnimInstance.generated.h"

/**
 * Kwang Animation Instance class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API UKwangAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// Initalise Kwang character animation 
	virtual void NativeInitializeAnimation() override;

	// Update Kwang character animation every frame
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// Reference to the Kwang character instance
	UPROPERTY(BlueprintReadOnly)
	class AKwang* Kwang;

	// Reference to the Kwang character's movement component
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* KwangMovement;

	// Ground speed of the character
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	// Flag indicating whether the character is falling
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;

	// Character state enum indicating the current state of the character
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Character State")
	ECharacterState CharacterState;
};
