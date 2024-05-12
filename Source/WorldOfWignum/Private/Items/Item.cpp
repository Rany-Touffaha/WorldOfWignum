// World of Wignum by Rany Touffaha

#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"

/**
 * Item class constructor
 */
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a default sub-object for the item's mesh component and attach it to the root
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	// Create a default sub-object for the sphere component and attach it to the root
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	// Create a default sub-object for the Niagara component and attach it to the root
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap events
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

// Transforms Sin function using Amplitude and TimeConstant
float AItem::TransformedSin() const
{
	return Amplitude* FMath::Sin(RunningTime * TimeConstant);
}

// Transforms Cos function using Amplitude and TimeConstant
float AItem::TransformedCos() const
{
	return Amplitude* FMath::Cos(RunningTime * TimeConstant);;
}

// Function that sets overlapping item to the character class
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is a pickup interface
	if(IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
	{
		// Set overlapping item for the pickup interface
		PickupInterface->SetOverlappingItem(this);
	}
}

// Function that removes overlapping item to the character class
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the overlapping actor is a pickup interface
	if(IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
	{
		// Clear overlapping item for the pickup interface
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
			);
	} 
}

void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the running time for the sine and cosine functions
	RunningTime += DeltaTime;

	// Add sinusodial movement if in hovering state
	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}

