#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"


class UAuraUserWidget;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;


/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const struct FWidgetControllerParams& params);

	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);

protected:

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
