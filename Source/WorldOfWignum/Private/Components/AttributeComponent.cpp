// World of Wignum by Rany Touffaha

#include "Components/AttributeComponent.h"

/**
 * Attribute component class constructor
 */
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina+StaminaRegenRate*DeltaTime, 0.f, MaxStamina);
}

// Reduces health from bound object
void UAttributeComponent::ReceiveDamage(const float Damage)
{
	Health = FMath::Clamp(Health-Damage, 0.f, MaxHealth);
}

void UAttributeComponent::UseStamina(const float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina-StaminaCost, 0.f, MaxStamina);
}

// Getter for health percentage
float UAttributeComponent::GetHealthPercent() const
{
	return Health/MaxHealth;
}

float UAttributeComponent::GetStaminaPercent() const
{
	return Stamina/MaxStamina;
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
