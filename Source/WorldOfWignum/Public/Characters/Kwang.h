#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Kwang.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class WORLDOFWIGNUM_API AKwang : public ACharacter
{
	GENERATED_BODY()

public:
	AKwang();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* KwangContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
private:

	// Spring arm component for camera control
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	// Camera component for character's view
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
};
 