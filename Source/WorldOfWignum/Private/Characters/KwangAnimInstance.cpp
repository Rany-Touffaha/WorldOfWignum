// World of Wignum by Rany Touffaha

#include "Characters/KwangAnimInstance.h"
#include "Characters/Kwang.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

/**
 * Initialise animation instance
 */
void UKwangAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Kwang = Cast<AKwang>(TryGetPawnOwner());
	if (Kwang)
		KwangMovement = Kwang->GetCharacterMovement();
}

/**
 * Update animation instance every frame
 * @param DeltaTime Number of frames per second
 */
void UKwangAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(KwangMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(KwangMovement->Velocity);
		IsFalling = KwangMovement->IsFalling();
		CharacterState = Kwang->GetCharacterState();
		ActionState = Kwang->GetActionState();
		DeathPose = Kwang->GetDeathPose();
	}
}
