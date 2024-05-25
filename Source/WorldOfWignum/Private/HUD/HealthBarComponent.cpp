// World of Wignum by Rany Touffaha

#include "HUD/HealthBarComponent.h"
#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"

/**
 * Setter for percetage health bar widget
 * @param Percent Health percentage to be set
 */
void UHealthBarComponent::SetHealthPercent(const float Percent)
{
	if(HealthBarWidget == nullptr)
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());

	if (HealthBarWidget && HealthBarWidget->HealthBar)
		HealthBarWidget->HealthBar->SetPercent(Percent);
}
