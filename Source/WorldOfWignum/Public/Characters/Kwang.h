#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Kwang.generated.h"

class UInputMappingContext;
class UInputAction;

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

	void Move(const FInputActionValue& Value);
};
