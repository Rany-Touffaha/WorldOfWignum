// World of Wignum by Rany Touffaha

#include "Characters/Kwang.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "HUD/WignumHUD.h"
#include "HUD/WignumOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

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

	// Set collision responses for capsule component
	GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// Set collision responses for character mesh
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

	// Update UI for Kwang's stamina
	if (Attributes && WignumOverlay)
	{
		Attributes->RegenStamina(DeltaSeconds);
		WignumOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

/**
 * Sets up the player's input controller by binding the input actions to corresponding functions
 * @param PlayerInputComponent Player's current controller
 */
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

/**
 * Callback function to handle jump movement
 */
void AKwang::Jump()
{
	if (IsUnoccupied())
		Super::Jump();
}

/**
 * Takes damage from Kwang's health
 * @param DamageAmount Amount of damage to be taken
 * @param DamageEvent Event type used by UE
 * @param EventInstigator Controller that instigates the event
 * @param DamageCauser Actor that causes the damage
 * @return Amount of damage to be taken from Kwang's health
 */
float AKwang::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

/**
 * Handles Kwang's reaction when getting hit using IHitInterface 
 * @param ImpactPoint Location of impact point of the hit
 * @param Hitter Actor that is hitting Kwang
 */
void AKwang::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Attributes && Attributes->GetHealthPercent() > 0.f)
		ActionState = EActionState::EAS_HitReaction;
}

/**
 * Sets the overlapping item variable
 * @param Item Item that is overlapping with Kwang
 */
void AKwang::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

/**
 * Adds souls to the attributes and updates the UI
 * @param Soul Soul that was picked up in the world
 */
void AKwang::AddSouls(ASoul* Soul)
{
	if (Attributes && WignumOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		WignumOverlay->SetSouls(Attributes->GetSouls());
	}
}

/**
 * Adds gold to the attributes and updates the UI
 * @param Treasure Treasure that was picked up in the world
 */
void AKwang::AddGold(ATreasure* Treasure)
{
	if (Attributes && WignumOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		WignumOverlay->SetGold(Attributes->GetGold());
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

/**
 * Callback function to handle movement input
 * @param Value Input action value received from the controller
 */
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

/**
 * Callback function to handle looking around input
 * @param Value Input action value received from the controller
 */
void AKwang::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

/**
 * Callback function to handle E key action
 */
void AKwang::EKeyPressed()
{
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		if (EquippedWeapon)
			EquippedWeapon->Destroy();

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

/**
 * Callback function to handle attack action
 */
void AKwang::Attack()
{
	Super::Attack();
	
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

/**
 * Callback function to handle dodge action
 */
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

/**
 * Equips weapon to the right hand socket of Kwang
 * @param Weapon Overlapping weapon object with Kwang in the world
 */
void AKwang::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

/**
 * Checks if Kwang can attack
 * @return Action state of character is unoccupied and character state is anything but unequipped
 */
bool AKwang::CanAttack() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
		
}

/**
 * Changes action state to unoccupied at the end of an attack
 */
void AKwang::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

/**
 * Changes action state to unoccupied at the end of a dodge
 */
void AKwang::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

/**
 * Checks if character can disarm 
 * @return Action state of character is unoccupied and character state is anything but unequipped
 */
bool AKwang::CanDisarm() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

/**
 * Checks if character can arm 
 * @return Action state of character is unoccupied and character state is unequipped and there is an equipped weapon
 */
bool AKwang::CanArm() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

/**
 * Disarms Kwang
 */
void AKwang::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

/**
 * Arms Kwang
 */
void AKwang::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

/**
 * Plays Equip Montage based on section name
 * @param SectionName Name of section in the Equip animation montage to be played
 */
void AKwang::PlayEquipMontage(const FName& SectionName) const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

/**
 * Initiates Kwang's death.
 */
void AKwang::Die_Implementation()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

/**
 * Checks if Kwang's action state is unoccupied
 * @return true if action state of character is not unoccupied, false otherwise
 */
bool AKwang::IsOccupied() const
{
	return ActionState != EActionState::EAS_Unoccupied;
}

/**
 * Checks if Kwang has enough stamina
 * @return true if the character's current stamina is larger than the dodge cost, false otherwise
 */
bool AKwang::HasEnoughStamina() const
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

/**
 * Attaches the weapon to the back of the Kwang
 */
void AKwang::AttachWeaponToBack() const
{
	if (EquippedWeapon)
		EquippedWeapon -> AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
}

/**
 * Attaches he weapon to the right hand of the character
 */
void AKwang::AttachWeaponToHand() const
{
	if (EquippedWeapon)
		EquippedWeapon -> AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
}

/**
 * Reset the state to unoccupied when done equipping
 */
void AKwang::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

/**
 * Reset the state to unoccupied after reacting to getting hit
 */
void AKwang::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

/**
 * Initalises HUD overlay
 * @param PlayerController Player's current controller
 */
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

/**
 * Initialises the input mapping context
 * @param PlayerController Player's current controller
 */
void AKwang::InitialiseInputMappingContext(const APlayerController* PlayerController) const
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		Subsystem->AddMappingContext(KwangContext,0);
}

/**
 * Setup health bar percetage in HUD
 */
void AKwang::SetHUDHealth() const
{
	if (WignumOverlay && Attributes)
		WignumOverlay->SetHealtBarPercent(Attributes->GetHealthPercent());
}

/**
 * Checks if the character is unoccupied
 * @return true if action state of character is unoccupied, false otherwise
 */
bool AKwang::IsUnoccupied() const
{
	return ActionState == EActionState::EAS_Unoccupied;
}