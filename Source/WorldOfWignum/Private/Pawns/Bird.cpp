#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set up the capsule component for collision
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	// Set up the skeletal mesh component for the bird
	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	// Set up the spring arm component for camera control
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	// Set up the camera component for the bird's view
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	// Auto possess this pawn by Player0
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	// Add the bird's input mapping context to the Enhanced Input subsystem
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext,0);
		}
	}
}

// Function to handle bird movement
void ABird::Move(const FInputActionValue& Value)
{
	if(const float DirectionValue = Value.Get<float>(); Controller && DirectionValue != 0.f)
		{
			// Move the bird along its forward vector
			const FVector Forward = GetActorForwardVector();
			AddMovementInput(Forward, DirectionValue);
		}
}

// Function to handle bird looking
void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if(GetController())
	{
		// Rotate the bird's controller to look around
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind the bird's movement and look functions to input actions using Enhanced Input
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}
}

