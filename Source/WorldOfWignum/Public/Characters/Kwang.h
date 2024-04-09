#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Kwang.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;

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

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void EKeyPressed();
	void Attack();
	void Dodge();
	
private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	// Spring arm component for camera control
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	// Camera component for character's view
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	
public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
 