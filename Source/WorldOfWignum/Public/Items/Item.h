// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

//Forward declarations for Item class
class USphereComponent;
class UNiagaraComponent;

/**
 * Enum class declaration for item states
 */
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

/**
 * Item class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// Property for amplitude of sine function
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25f;

	// Property for time constant of sine function
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	// Static mesh component for the item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	// Sphere component for the item
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	// Niagara effect component for the item
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffect;

	// State of the item
	EItemState ItemState = EItemState::EIS_Hovering;

	// Blueprint pure function to calculate transformed sine
	UFUNCTION(BlueprintPure)
	float TransformedSin() const;

	// Blueprint pure function to calculate transformed cosine
	UFUNCTION(BlueprintPure)
	float TransformedCos() const;
	
	// Function to handle sphere overlap events
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function to handle events when leaving sphere overlap
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	// Property to track running time
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime = 0;
};

