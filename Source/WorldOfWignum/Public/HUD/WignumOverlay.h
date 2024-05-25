// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WignumOverlay.generated.h"

//Forward declarations for Wignum overlay class
class UTextBlock;
class UProgressBar;

UCLASS()
class WORLDOFWIGNUM_API UWignumOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealtBarPercent(float Percent) const;
	void SetStaminaBarPercent(float Percent) const;
	void SetGold(int32 Gold) const;
	void SetSouls(int32 Souls) const;

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulsText;
};
