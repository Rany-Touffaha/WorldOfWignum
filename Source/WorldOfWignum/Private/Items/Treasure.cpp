// World of Wignum by Rany Touffaha

#include "Items/Treasure.h"

#include "Interfaces/PickupInterface.h"

/**
 * Picks up the overlapping treasure
 * @param OverlappedComponent paramter not in use since function is an override from parent class
 * @param OtherActor paramter not in use since function is an override from parent class
 * @param OtherComp paramter not in use since function is an override from parent class
 * @param OtherBodyIndex paramter not in use since function is an override from parent class
 * @param bFromSweep paramter not in use since function is an override from parent class
 * @param SweepResult paramter not in use since function is an override from parent class
 */
void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
	{
		PickupInterface->AddGold(this);
		SpawnPickupSound();
		Destroy();
	}
}
