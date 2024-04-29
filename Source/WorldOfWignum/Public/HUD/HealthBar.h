// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

//Forward declarations for HealthBar class
class UProgressBar;

/**
 * Health Bar class declaration
 */
UCLASS()
class WORLDOFWIGNUM_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	// Progress bar widget within the health bar widget
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
