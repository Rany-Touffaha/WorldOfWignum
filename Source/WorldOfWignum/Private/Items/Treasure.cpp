// World of Wignum by Rany Touffaha

#include "Items/Treasure.h"
#include "Characters/Kwang.h"
#include "Kismet/GameplayStatics.h"

// Function that picks up the overlapping treasure
void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is a character
	if(AKwang* KwangCharacter = Cast<AKwang>(OtherActor))
	{
		// Play pickup sound
		if(PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}

		// Destroy the treasure when picked up
		Destroy();
	}
}
