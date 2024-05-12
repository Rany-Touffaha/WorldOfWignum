// World of Wignum by Rany Touffaha


#include "Items/Soul.h"
#include "Interfaces/PickupInterface.h"

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is a pickup interface
	if(IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
	{
		// Set overlapping item for the pickup interface
		PickupInterface->AddSouls(this);
	}

	Destroy();
}
