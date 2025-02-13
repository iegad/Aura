#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

static const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Message")));

void
UOverlayWidgetController::BroadcastInitialValues()
{
	const auto* auraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(auraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(auraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(auraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(auraAttributeSet->GetMaxMana());
}

void 
UOverlayWidgetController::BindCallbacksToDependencies()
{
	const auto* auraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	auto* auraAbilitySystemComp = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	auraAbilitySystemComp->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags) {
			for (auto& tag : AssetTags) {
				if (tag.MatchesTag(MessageTag)) {
					const auto* row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, tag);
					MessageWidgetRowDelegate.Broadcast(*row);
				}
			}
		}
	);
}
