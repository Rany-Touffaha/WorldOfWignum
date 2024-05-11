// World of Wignum by Rany Touffaha

#include "HUD/WignumHUD.h"
#include "Blueprint/UserWidget.h"
#include "HUD/WignumOverlay.h"

void AWignumHUD::BeginPlay()
{
	Super::BeginPlay();

	if (const UWorld* World = GetWorld())
	{
		APlayerController* Controller = World->GetFirstPlayerController();

		if (Controller && WignumOverlayClass)
		{
			WignumOverlay = CreateWidget<UWignumOverlay>(Controller, WignumOverlayClass);
			WignumOverlay->AddToViewport();
		}
	}
}
