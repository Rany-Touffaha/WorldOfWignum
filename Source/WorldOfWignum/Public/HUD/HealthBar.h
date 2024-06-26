// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

//Forward declarations for HealthBar class
class UProgressBar;

UCLASS()
class WORLDOFWIGNUM_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
