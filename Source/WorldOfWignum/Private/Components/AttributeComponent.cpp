// World of Wignum by Rany Touffaha

#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
 Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/**
 * Updates stamina regeneration in attributes component
 * @param DeltaTime Number of frames per second
 */
void UAttributeComponent::RegenStamina(float DeltaTime)
{
 Stamina = FMath::Clamp(Stamina+StaminaRegenRate*DeltaTime, 0.f, MaxStamina);
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Reduces health from bound object
 * @param Damage Amount of damage to be received
 */
void UAttributeComponent::ReceiveDamage(const float Damage)
{
 Health = FMath::Clamp(Health-Damage, 0.f, MaxHealth);
}

/**
 * Uses stamina whenever the character dodges
 * @param StaminaCost Amount of stamina to be removed
 */
void UAttributeComponent::UseStamina(const float StaminaCost)
{
 Stamina = FMath::Clamp(Stamina-StaminaCost, 0.f, MaxStamina);
}

/**
 * Getter for health percentage in component
 * @return percentage of health left in component
 */
float UAttributeComponent::GetHealthPercent() const
{
 return Health/MaxHealth;
}

/**
 * Getter for stamina percentage in component
 * @return percentage of stamina left in component
 */
float UAttributeComponent::GetStaminaPercent() const
{
 return Stamina/MaxStamina;
}

/**
 * Check if character's health is above 0
 * @return true if alive, false otherwise
 */
bool UAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

/**
 * Add gold to the attribute component
 * @param AmountOfGold Amount of gold to be added to component
 */
void UAttributeComponent::AddGold(const int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

/**
 * Add souls to the attribute component
 * @param NumberOfSouls Amount of souls to be added to component
 */
void UAttributeComponent::AddSouls(const int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}
