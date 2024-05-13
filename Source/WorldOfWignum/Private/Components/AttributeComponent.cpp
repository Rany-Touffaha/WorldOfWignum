// World of Wignum by Rany Touffaha

#include "Components/AttributeComponent.h"

/**
 * Attribute component class constructor
 */
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Reduces health from bound object
void UAttributeComponent::ReceiveDamage(const float Damage)
{
	Health = FMath::Clamp(Health-Damage, 0.f, MaxHealth);
}

// Getter for health percentage
float UAttributeComponent::GetHealthPercent() const
{
	return Health/MaxHealth;
}

// Check if character's health is above 0
bool UAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

void UAttributeComponent::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}
