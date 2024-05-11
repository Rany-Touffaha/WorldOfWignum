// World of Wignum by Rany Touffaha


#include "HUD/WignumOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWignumOverlay::SetHealtBarPercent(float Percent) const
{
	if (HealthProgressBar)
		HealthProgressBar->SetPercent(Percent);
	
}

void UWignumOverlay::SetStaminaBarPercent(float Percent) const
{
	if (StaminaProgressBar)
		StaminaProgressBar->SetPercent(Percent);
}

void UWignumOverlay::SetGold(int32 Gold) const
{
	if (GoldText)
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
}

void UWignumOverlay::SetSouls(int32 Souls) const
{
	if (SoulsText)
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
}
