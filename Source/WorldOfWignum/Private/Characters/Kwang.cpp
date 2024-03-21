#include "Characters/Kwang.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AKwang::AKwang()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKwang::BeginPlay()
{
	Super::BeginPlay();

	// Add the bird's input mapping context to the Enhanced Input subsystem
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(KwangContext,0);
		}
	}	
}

void AKwang::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, MovementVector.Y);
	const FVector Right = GetActorRightVector();
	AddMovementInput(Right, MovementVector.X);
}

void AKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKwang::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind the bird's movement and look functions to input actions using Enhanced Input
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AKwang::Move);

	}
}

