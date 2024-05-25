// World of Wignum by Rany Touffaha

#include "HUD/WignumOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


/**
 * Setter for percentage in health bar
 * @param Percent Percentage of health to be set
 */
void UWignumOverlay::SetHealtBarPercent(const float Percent) const
{
	if (HealthProgressBar)
		HealthProgressBar->SetPercent(Percent);
	
}

/**
 * Setter for percentage in stamina bar
 * @param Percent Percentage of stamina to be set
 */
void UWignumOverlay::SetStaminaBarPercent(const float Percent) const
{
	if (StaminaProgressBar)
		StaminaProgressBar->SetPercent(Percent);
}

/**
 * Setter for amount of gold in HUD
 * @param Gold Amount of gold to be set
 */
void UWignumOverlay::SetGold(const int32 Gold) const
{
	if (GoldText)
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
}

/**
 * Setter for amount of souls in HUD
 * @param Souls Amount of souls to be set
 */
void UWignumOverlay::SetSouls(const int32 Souls) const
{
	if (SoulsText)
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
}
