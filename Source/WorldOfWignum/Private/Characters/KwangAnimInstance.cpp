// World of Wignum by Rany Touffaha

#include "Characters/KwangAnimInstance.h"
#include "Characters/Kwang.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Initialise animation instance
void UKwangAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Get the owning pawn
	Kwang = Cast<AKwang>(TryGetPawnOwner());
	if (Kwang)
	{
		// Get the character movement component
		KwangMovement = Kwang->GetCharacterMovement();
	}
}

// Update animation instance every frame
void UKwangAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(KwangMovement)
	{
		// Calculate ground speed using velocity
		GroundSpeed = UKismetMathLibrary::VSizeXY(KwangMovement->Velocity);

		// Check if the character is falling
		IsFalling = KwangMovement->IsFalling();

		// Get the character's current state
		CharacterState = Kwang->GetCharacterState();
	}
}
