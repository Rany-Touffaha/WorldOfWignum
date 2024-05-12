// World of Wignum by Rany Touffaha

#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"

/**
 * Weapon class constructor
 */
AWeapon::AWeapon()
{
	// Create a box component and set up collsion responses
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Create the start component of the box trace
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	// Create the end component of the box trace
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Bind the OnBoxOverlap function to the weapon box component when overlapping
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

// Function to attach an item mesh to a socket
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const
{
	const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::PlayEquipSound() const
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
}

void AWeapon::DisableSphereCollision() const
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::DeactivateEmbers() const
{
	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

// Function to equip the weapon to a parent scene component at a specific socket
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	// Set the Item State to equipped
	ItemState = EItemState::EIS_Equipped;
	
	// Set owner and instigator
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	
	// Attach mesh to a socket
	AttachMeshToSocket(InParent, InSocketName);

	// Disable the collision of the character sphere with the weapon when equipping to the back
	DisableSphereCollision();
	
	// Play sound when equipping
	PlayEquipSound();
	
	// Disable Niagara effect once the weapon is picked up
	DeactivateEmbers();
}

void AWeapon::ExecuteGetHit(const FHitResult& BoxHit) const
{
	// Execute get hit if the actor is valid
	if(const IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor()))
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

bool AWeapon::ActorIsSameType(const AActor* OtherActor) const
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

// Create a box trace if the weapon overlaps with another component
void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorIsSameType(OtherActor)) return;;
	
	FHitResult BoxHit;
	BoxTrace(BoxHit);

	// Get the impact point of what the weapon hits only if it is a Hit interface 
	if (BoxHit.GetActor())
	{
		if(ActorIsSameType(BoxHit.GetActor())) return;
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage,GetInstigator()->GetController(),this, UDamageType::StaticClass());
		ExecuteGetHit(BoxHit);
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	// Create start and end point of the box trace
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	// Create a list of actors that should be ignored by the weapon when hitting them
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	// Loop over the list of ignore actors and add them to list of actors to ignore in the box trace 
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	// Create a box trace at the hit point of the weapon
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);

	// Add what has been hit to the list of actors are ignored
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
