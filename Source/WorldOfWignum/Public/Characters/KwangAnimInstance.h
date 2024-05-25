// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "KwangAnimInstance.generated.h"

UCLASS()
class WORLDOFWIGNUM_API UKwangAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	/** Kwang's attributes*/
	
	UPROPERTY(BlueprintReadOnly)
	class AKwang* Kwang;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Character State")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement|Action State")
	EActionState ActionState;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Death State")
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* KwangMovement;

	// Ground speed of the character
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	// Check whether the character is falling
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;


};
