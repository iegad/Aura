#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
}

void 
AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void 
AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	auto* targetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	if (!targetASC) {
		return;
	}

	check(GameplayEffectClass);

	auto effectContextHandle = targetASC->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);

	// GameplayEffectSpec (GESpec) 可以认为是 GameplayEffects 的实例化
	// UAbilitySystemComponent::MakeOutgoingSpec() 专门用于从 GameplayEffects 创建 GameplayEffectSpecs
	// 在应用一个GameplayEffect时, 会先从GameplayEffect中创建一个GameplayEffectSpec出来, 然后实际上是把GameplayEffectSpec应用给目标
	const auto effectSpecHandle = targetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, effectContextHandle);
	const auto activeEffectHandle = targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data);

	if (effectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite &&
		InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		ActiveEffectHandles.Add(activeEffectHandle, targetASC);
	}
}

void
AAuraEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap) {
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void
AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		auto targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetActor)) {
			return;
		}

		TArray<FActiveGameplayEffectHandle> handlesToRemove;
		for (auto handlePair : ActiveEffectHandles) {
			if (targetASC == handlePair.Value) {
				targetASC->RemoveActiveGameplayEffect(handlePair.Key, 1);
				handlesToRemove.Add(handlePair.Key);
			}
		}

		for (auto& handle : handlesToRemove) {
			ActiveEffectHandles.FindAndRemoveChecked(handle);
		}
	}
}
