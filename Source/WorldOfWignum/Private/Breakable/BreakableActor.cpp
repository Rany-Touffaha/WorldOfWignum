// World of Wignum by Rany Touffaha

#include "Breakable/BreakableActor.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"


ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create geometry collection component and set up its collision responses
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeomtryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Create capsule component and set up its collision responses
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
* Handles breakable item reaction when getting hit using IHitInterface 
* @param ImpactPoint Location of impact point of the hit
* @param Hitter Actor that is hitting the breakable object
*/
void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken) return;
	bBroken = true;
	if(UWorld* World = GetWorld(); World && TreasureClassArray.Num() > 0 )
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;

		// Show a treasure from the list when broken
		const int32 Selection = FMath::RandRange(0, TreasureClassArray.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClassArray[Selection], Location, GetActorRotation(), FActorSpawnParameters());
	}
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}


