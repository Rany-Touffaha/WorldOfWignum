#include "Items/Item.h"
#include "WorldOfWignum/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

//Transforms Sin function using Amplitude and TimeConstant
float AItem::TransformedSin() const
{
	return Amplitude* FMath::Sin(RunningTime * TimeConstant);
}

//Transforms Cos function using Amplitude and TimeConstant
float AItem::TransformedCos() const
{
	return Amplitude* FMath::Cos(RunningTime * TimeConstant);;
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));

	DRAW_SPHERE_SINGLE_FRAME(GetActorLocation())
	DRAW_VECTOR_SINGLE_FRAME(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f)
}

