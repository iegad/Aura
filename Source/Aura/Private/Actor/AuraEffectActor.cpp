#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


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
	auto* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!targetASC) {
		return;
	}

	

	check(GameplayEffectClass);

	auto effectContextHandle = targetASC->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);

	// GameplayEffectSpec (GESpec) ������Ϊ�� GameplayEffects ��ʵ����
	// UAbilitySystemComponent::MakeOutgoingSpec() ר�����ڴ� GameplayEffects ���� GameplayEffectSpecs
	// ��Ӧ��һ��GameplayEffectʱ, ���ȴ�GameplayEffect�д���һ��GameplayEffectSpec����, Ȼ��ʵ�����ǰ�GameplayEffectSpecӦ�ø�Ŀ��
	const auto effectSpecHandle = targetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, effectContextHandle);
	const auto activeEffectHandle = targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());

	const bool bIsInfinite = effectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
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
