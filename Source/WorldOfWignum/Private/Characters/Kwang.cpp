#include "Characters/Kwang.h"


AKwang::AKwang()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKwang::BeginPlay()
{
	Super::BeginPlay();
}

void AKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKwang::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

