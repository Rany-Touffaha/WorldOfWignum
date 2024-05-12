// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 *  Treasure class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API ATreasure : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	
	// Amount of gold with character
	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;
};
