// World of Wignum by Rany Touffaha

#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"

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

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap events
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

/**
 * Spawns pick up effect for the current item
 */
void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());
}

/**
 * Spawns pick up sound for the current item
 */
void AItem::SpawnPickupSound()
{
	if (PickupSound)
		UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation());
}

/**
 * Transforms Sin function using Amplitude and TimeConstant
 * @return sinusodial result of the amplitude and time constant
 */
float AItem::TransformedSin() const
{
	return Amplitude* FMath::Sin(RunningTime * TimeConstant);
}

/**
 * Transforms Cos function using Amplitude and TimeConstant
 * @return cosinusodial result of the amplitude and time constant
 */
float AItem::TransformedCos() const
{
	return Amplitude* FMath::Cos(RunningTime * TimeConstant);;
}

/**
 * Sets overlapping item to the current class
 * @param OverlappedComponent paramter not in use since function is an override from parent class
 * @param OtherActor paramter not in use since function is an override from parent class
 * @param OtherComp paramter not in use since function is an override from parent class
 * @param OtherBodyIndex paramter not in use since function is an override from parent class
 * @param bFromSweep paramter not in use since function is an override from parent class
 * @param SweepResult paramter not in use since function is an override from parent class
 */
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
		PickupInterface->SetOverlappingItem(this);
}

/**
 * Removes overlapping item from current class
 * @param OverlappedComponent paramter not in use since function is an override from parent class
 * @param OtherActor paramter not in use since function is an override from parent class
 * @param OtherComp paramter not in use since function is an override from parent class
 * @param OtherBodyIndex paramter not in use since function is an override from parent class
 */
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
		PickupInterface->SetOverlappingItem(nullptr);
}