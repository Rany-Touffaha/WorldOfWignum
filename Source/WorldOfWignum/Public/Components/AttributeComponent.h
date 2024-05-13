// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

/**
 * Attribute Component class declaration
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WORLDOFWIGNUM_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	// Current Health
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health = 100;

	// Maximum Health
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

public:
	// Reduces health from bound object
	void ReceiveDamage(float Damage);

	// Getter for health percentage
	float GetHealthPercent() const;

	// Check if character's health is above 0
	bool IsAlive() const;

	void AddGold(int32 AmountOfGold);
	void AddSouls(int32 NumberOfSouls);

	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
};
