// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

// Forward declarations for Soul class
class UNiagaraSystem;

UCLASS()
class WORLDOFWIGNUM_API ASoul : public AItem
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	double DesiredZ;

	UPROPERTY(EditAnywhere)
	float DriftRate = -15.f;
	
	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls;

public:
	/** Getters and Setters */
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(const int32 NumberOfSouls) { Souls = NumberOfSouls; }
};
