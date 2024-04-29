// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

// Forward declarations for Weapon class
class USoundBase;
class UBoxComponent;

/**
 * Weapon class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	
	// Function to attach an item mesh to a socket
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const;
	
	// Function to equip the weapon to a parent scene component at a specific socket
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	// List of actors to ignore in the box trace
	TArray<AActor*> IgnoreActors;
	
protected:
	virtual void BeginPlay() override;
	
	// Override function to handle sphere overlap events
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// Override function to handle events when leaving sphere overlap
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Create a box trace if the weapon overlaps with another component
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Create a force field that would break an object with the weapon
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	// Sound component for equipping weapon
	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	USoundBase* EquipSound;

	// Box component for detecting collision
	UPROPERTY(VisibleAnywhere, Category= "Weapon Properties")
	UBoxComponent* WeaponBox;
	
	// Component to mark the start of the box trace
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	// Component to mark the end of the box trace
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	// Amount of damage the weapon makes
	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	float Damage = 20.f;

public:
	// Getter for weapon box component
	FORCEINLINE UBoxComponent* GetWeaponBox() const {return WeaponBox; }
};
