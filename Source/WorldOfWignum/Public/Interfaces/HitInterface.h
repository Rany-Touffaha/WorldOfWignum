// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Hit interface declaration
 */
class WORLDOFWIGNUM_API IHitInterface
{
	GENERATED_BODY()

public:
	// Function declaration for the object getting hit
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};
