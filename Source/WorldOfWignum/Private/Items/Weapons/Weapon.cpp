// World of Wignum by Rany Touffaha

#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"

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

/**
 * Attach an item mesh to a socket
 * @param InParent Scene component that becomes the parent of the weapon
 * @param InSocketName Name of socket to attach the weapon
 */
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName InSocketName) const
{
	const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

/**
 * Plays equip sound
 */
void AWeapon::PlayEquipSound() const
{
	if (EquipSound)
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
}

/**
 * Disables sphere collsion
 */
void AWeapon::DisableSphereCollision() const
{
	if (Sphere)
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/**
 * Deactivate embers effect for weapon
 */
void AWeapon::DeactivateEmbers() const
{
	if (ItemEffect)
		ItemEffect->Deactivate();
}

/**
 * Equips the weapon to a parent scene component at a specific socket
 * @param InParent Scene component that becomes the parent of the weapon
 * @param InSocketName Name of socket to attach the weapon
 * @param NewOwner Actor that is the new owner of the weapon
 * @param NewInstigator Pawn that is the instigator of the weapon
 */
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
	PlayEquipSound();
	DeactivateEmbers();
}

/**
 * Execute get hit if the actor is valid
 * @param BoxHit Hit result for the box trace
 */
void AWeapon::ExecuteGetHit(const FHitResult& BoxHit) const
{
	if(const IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor()))
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
}

/**
 * Checks if the overlapping actor has an Enemy tag or not
 * @param OtherActor Actor that has overlappoed with the weapon
 * @return true of the actor has an enemy tag, false otherwise
 */
bool AWeapon::ActorIsSameType(const AActor* OtherActor) const
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Bind the OnBoxOverlap function to the weapon box component when overlapping
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

/**
 * Creates a box trace if the weapon overlaps with another component
 * @param OverlappedComponent paramter not in use since function is an override from parent class
 * @param OtherActor paramter not in use since function is an override from parent class
 * @param OtherComp paramter not in use since function is an override from parent class
 * @param OtherBodyIndex paramter not in use since function is an override from parent class
 * @param bFromSweep paramter not in use since function is an override from parent class
 * @param SweepResult paramter not in use since function is an override from parent class
 */
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

/**
 * Creates a box hit trace for the weapon 
 * @param BoxHit Hit result for the box trace
 */
void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	// Create start and end point of the box trace
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	// Create a list of actors that should be ignored by the weapon when hitting them
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

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
