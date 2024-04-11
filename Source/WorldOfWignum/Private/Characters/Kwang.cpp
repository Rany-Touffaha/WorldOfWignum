#include "Characters/Kwang.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"

// Kwang character class constructor
AKwang::AKwang()
{
	PrimaryActorTick.bCanEverTick = true;

	// Remove inheritance of Character's class rotation from the Controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Make character rotate towards our movement
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Change the speed of the Rotation Rate
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// Set up the spring arm component for camera control
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	// Set up the camera component for the character's view
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
}

void AKwang::BeginPlay()
{
	Super::BeginPlay();

	// Add the player's input mapping context to the Enhanced Input subsystem
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(KwangContext,0);
		}
	}	
}

// Function to handle movement input
void AKwang::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// Get Yaw of Controller Rotator
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	// Add Movement for character to move forward and backward
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	// Add Movement for character to move sideways
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

// Function to handle looking around input
void AKwang::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

// Function to handle E key action
void AKwang::EKeyPressed()
{
	if (const AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		//Attaching Weapon to Right Hand Socket
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));

		//Change Character state to equipped one-handed
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
}

// Function to handle attack action
void AKwang::Attack()
{
	
}

// Function to handle dodge action
void AKwang::Dodge()
{
	
}

void AKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Override SetupPlayerInputComponent function to bind input actions to corresponding functions
void AKwang::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind the character's movement and look functions to input actions using Enhanced Input
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AKwang::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKwang::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &AKwang::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AKwang::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AKwang::Dodge);
	}
}

