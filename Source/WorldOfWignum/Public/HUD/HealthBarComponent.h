// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

//Forward declarations for HealthBarComponent class
class UHealthBar;

UCLASS()
class WORLDOFWIGNUM_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetHealthPercent(float Percent);
	
private:
	UPROPERTY()
	UHealthBar* HealthBarWidget;
};
