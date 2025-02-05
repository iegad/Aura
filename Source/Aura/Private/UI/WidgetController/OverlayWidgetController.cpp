#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"


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

	auto& healthDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetHealthAttribute());
	healthDelegate.AddUObject(this, &ThisClass::HealthChanged);

	auto& maxHealthDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxHealthAttribute());
	maxHealthDelegate.AddUObject(this, &ThisClass::MaxHealthChanged);

	auto& manaDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetManaAttribute());
	manaDelegate.AddUObject(this, &ThisClass::ManaChanged);

	auto& maxManaDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxManaAttribute());
	maxManaDelegate.AddUObject(this, &ThisClass::MaxManaChanged);

	auto* auraAbilitySystemComp = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	auraAbilitySystemComp->EffectAssetTags.AddLambda(
		[](const FGameplayTagContainer& AssetTags) {
			for (auto& tag : AssetTags) {
				const FString msg = FString::Printf(TEXT("GE Tag: %s"), *tag.ToString());
				GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, msg);
			}
		}
	);
}

void
UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void
UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void 
UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void 
UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
