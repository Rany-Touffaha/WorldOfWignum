// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WignumHUD.generated.h"

//Forward declarations for WignnumHUD class
class UWignumOverlay;

UCLASS()
class WORLDOFWIGNUM_API AWignumHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Wignum")
	TSubclassOf<UWignumOverlay> WignumOverlayClass;

	UPROPERTY()
	UWignumOverlay* WignumOverlay;

public:
	/** Getters and Setters */
	FORCEINLINE UWignumOverlay* GetWignumOverlay() const { return WignumOverlay; }
};
