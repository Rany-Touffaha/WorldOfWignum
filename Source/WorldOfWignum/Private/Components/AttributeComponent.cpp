// World of Wignum by Rany Touffaha

#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health-Damage, 0.f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent() const
{
	return Health/MaxHealth;
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

