#include "UI/HUD/AuraHUD.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* 
AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& params)
{
	if (!OverlayWidgetController) {
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

void
AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uinitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uinitialized, please fill out BP_AuraHUD"));

	auto* widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	check(widget);

	OverlayWidget = Cast<UAuraUserWidget>(widget);
	check(OverlayWidget);

	const FWidgetControllerParams widgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	auto* overlayWidgetController = GetOverlayWidgetController(widgetControllerParams);

	OverlayWidget->SetWidgetController(overlayWidgetController);
	overlayWidgetController->BroadcastInitialValues();

	widget->AddToViewport();
}