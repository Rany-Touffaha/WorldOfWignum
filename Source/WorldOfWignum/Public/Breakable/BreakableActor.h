// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class WORLDOFWIGNUM_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

	// Function that handles breakable item reaction when getting hit
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;
};
