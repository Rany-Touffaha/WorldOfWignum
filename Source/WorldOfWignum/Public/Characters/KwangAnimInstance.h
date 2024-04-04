#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KwangAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WORLDOFWIGNUM_API UKwangAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class AKwang* Kwang;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* KwangMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	
};
