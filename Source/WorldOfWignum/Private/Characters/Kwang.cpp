// World of Wignum by Rany Touffaha

#include "Characters/Kwang.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "HUD/WignumHUD.h"
#include "HUD/WignumOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

/**
 * Kwang character class constructor
 */
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

	GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Set up the spring arm component for camera control
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	// Set up the camera component for the character's view
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
}

void AKwang::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Attributes && WignumOverlay)
	{
		Attributes->RegenStamina(DeltaSeconds);
		WignumOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AKwang::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &AKwang::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AKwang::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AKwang::Dodge);
	}
}

bool AKwang::IsUnoccupied() const
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void AKwang::Jump()
{
	if (IsUnoccupied())
		Super::Jump();
}

void AKwang::SetHUDHealth() const
{
	if (WignumOverlay && Attributes)
	{
		WignumOverlay->SetHealtBarPercent(Attributes->GetHealthPercent());
	}
}

float AKwang::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AKwang::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void AKwang::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AKwang::AddSouls(ASoul* Soul)
{
	if (Attributes && WignumOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		WignumOverlay->SetSouls(Attributes->GetSouls());
	}
}

void AKwang::AddGold(ATreasure* Treasure)
{
	if (Attributes && WignumOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		WignumOverlay->SetGold(Attributes->GetGold());
	}
}

void AKwang::InitialiseWignumOverlay(const APlayerController* PlayerController)
{
	if (const AWignumHUD* WignumHUD = Cast<AWignumHUD>(PlayerController->GetHUD()))
	{
		WignumOverlay = WignumHUD->GetWignumOverlay();

		if (WignumOverlay && Attributes)
		{
			WignumOverlay->SetHealtBarPercent(Attributes->GetHealthPercent());
			WignumOverlay->SetStaminaBarPercent(1.f);
			WignumOverlay->SetGold(0);
			WignumOverlay->SetSouls(0);
		}
	}
}

void AKwang::InitialiseInputMappingContext(const APlayerController* PlayerController) const
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(KwangContext,0);
	}
}

void AKwang::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngageableTarget"));

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		InitialiseWignumOverlay(PlayerController);
		InitialiseInputMappingContext(PlayerController);
	}
}

// Function to handle movement input
void AKwang::Move(const FInputActionValue& Value)
{
	// Check of action state is unoccupied before moving
	if(ActionState != EActionState::EAS_Unoccupied) return;
	
	// Get Yaw of Controller Rotator
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	// Add Movement for character to move forward and backward
	const FVector2D MovementVector = Value.Get<FVector2D>();
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
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if(CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

// Function to handle attack action
void AKwang::Attack()
{
	Super::Attack();
	
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool AKwang::IsOccupied() const
{
	return ActionState != EActionState::EAS_Unoccupied;
}

bool AKwang::HasEnoughStamina() const
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

// Function to handle dodge action
void AKwang::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && WignumOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		WignumOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void AKwang::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

// Function that checks if the character can attack
bool AKwang::CanAttack() const
{
	return CharacterState != ECharacterState::ECS_Unequipped &&
		ActionState == EActionState::EAS_Unoccupied;
}

// Function to change action state to unoccupied at the end of an attack
void AKwang::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AKwang::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

// Function that checks if character can disarm 
bool AKwang::CanDisarm() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

// Function that checks if character can arm 
bool AKwang::CanArm() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void AKwang::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AKwang::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

// Function that plays the equip montages
void AKwang::PlayEquipMontage(const FName& SectionName) const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AKwang::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

// Function to attach the weapon to the back of the character
void AKwang::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon -> AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

// Function to attach the weapon to the right hand of the character
void AKwang::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon -> AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

// Function to reset the state to unoccupied when done equipping
void AKwang::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AKwang::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
