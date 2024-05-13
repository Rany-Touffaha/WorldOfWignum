// World of Wignum by Rany Touffaha

#include "Items/Treasure.h"

#include "Interfaces/PickupInterface.h"

// Function that picks up the overlapping treasure
void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is a pickup interface
	if(IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
	{
		// Add to gold count from the treasure
		PickupInterface->AddGold(this);

		// Play pickup sound
		SpawnPickupSound();

		// Destroy the treasure when picked up
		Destroy();
	}
}
