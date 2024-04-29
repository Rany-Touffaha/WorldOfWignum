// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"


// Forward declarations for BreakableActor class
class UGeometryCollectionComponent;
class ATreasure;
class UCapsuleComponent;
 
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

	// Capsule component for breakable objects
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;

private:
	// Check if object is broken
	bool bBroken = false;

	// List of treasures inside the breakable object
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<ATreasure>> TreasureClassArray;
};
