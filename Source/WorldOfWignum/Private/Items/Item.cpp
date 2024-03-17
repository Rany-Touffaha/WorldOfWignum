#include "Items/Item.h"
#include "WorldOfWignum/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a default sub-object for the item's mesh
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));

	// Set the root component to the item's mesh
	RootComponent = ItemMesh;
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

	// Update the running time for the sine and cosine functions
	RunningTime += DeltaTime;

}

