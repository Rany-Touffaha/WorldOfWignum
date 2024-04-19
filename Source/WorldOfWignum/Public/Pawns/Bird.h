// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Bird.generated.h"

// Forward declarations for Bird Class
class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

/**
 * Bird class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;

	// Property for bird's input mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Input)
	UInputMappingContext* BirdMappingContext;

	// Property for bird's move action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Input)
	UInputAction* MoveAction;

	// Property for bird's look action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Input)
	UInputAction* LookAction;

	// Function to handle bird movement
	void Move(const FInputActionValue& Value);

	// Function to handle bird looking
	void Look(const FInputActionValue& Value);

private:
	// Capsule component for collision
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	// Skeletal mesh component for the bird
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;

	// Spring arm component for camera control
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	// Camera component for bird's view
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
};
