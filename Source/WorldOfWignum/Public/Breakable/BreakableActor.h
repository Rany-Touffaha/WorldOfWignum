// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"


class UCapsuleComponent;
//Forward declarations for BreakableActor class
class UGeometryCollectionComponent;
class ATreasure;

/**
 * Breakable Actor class declaration
 */
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

	// Geometric Collection Component for breakable objects
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;

private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATreasure> TreasureClass;
};
