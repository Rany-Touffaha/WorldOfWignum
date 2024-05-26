// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

// Forward declarations for Weapon class
class USoundBase;
class UBoxComponent;

UCLASS()
class WORLDOFWIGNUM_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const;
	void PlayEquipSound() const;
	void DisableSphereCollision() const;
	void DeactivateEmbers() const;
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void ExecuteGetHit(const FHitResult& BoxHit) const;
	bool ActorIsSameType(const AActor* OtherActor) const;

	// List of actors to ignore in the box trace
	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	bool bShowBoxDebug = false;
	
	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category= "Weapon Properties")
	UBoxComponent* WeaponBox;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	float Damage = 20.f;

public:
	/** Getters and Setters */
	FORCEINLINE UBoxComponent* GetWeaponBox() const {return WeaponBox; }
};
