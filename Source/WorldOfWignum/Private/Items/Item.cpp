#include "Items/Item.h"
#include "WorldOfWignum/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/Kwang.h"

//Item class constructor
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a default sub-object for the item's mesh
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));

	// Set the root component to the item's mesh
	RootComponent = ItemMesh;

	// Create a default sub-object for the sphere component and attach it to the root
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap events
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
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

// Function that sets overlapping item to the character class
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is a character
	if(AKwang* KwangCharacter = Cast<AKwang>(OtherActor))
	{
		// Set overlapping item for the character
		KwangCharacter->SetOverlappingItem(this);
	}
}

// Function that removes overlapping item to the character class
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the overlapping actor is a character
	if(AKwang* KwangCharacter = Cast<AKwang>(OtherActor))
	{
		// Clear overlapping item for the character
		KwangCharacter->SetOverlappingItem(nullptr);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the running time for the sine and cosine functions
	RunningTime += DeltaTime;

}

