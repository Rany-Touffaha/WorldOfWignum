// World of Wignum by Rany Touffaha

#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

/**
 * Breakable Actor class constructor
 */
ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeomtryCollection"));
	SetRootComponent(GeometryCollection);
	
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	// Function called in blueprint
}

