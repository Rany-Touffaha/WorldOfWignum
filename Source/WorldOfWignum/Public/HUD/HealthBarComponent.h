// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

//Forward declarations for HealthBarComponent class
class UHealthBar;

/**
 * Health Bar Component class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	// Setter for percetage health bar widget
	void SetHealthPercent(float Percent);
	
private:
	// Widget for health bar
	UPROPERTY()
	UHealthBar* HealthBarWidget;
};
