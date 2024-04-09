#include "Characters/KwangAnimInstance.h"
#include "Characters/Kwang.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UKwangAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Kwang = Cast<AKwang>(TryGetPawnOwner());
	if (Kwang)
	{
		KwangMovement = Kwang->GetCharacterMovement();
	}
}

void UKwangAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(KwangMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(KwangMovement->Velocity);
		IsFalling = KwangMovement->IsFalling();
		CharacterState = Kwang->GetCharacterState();
	}
}
