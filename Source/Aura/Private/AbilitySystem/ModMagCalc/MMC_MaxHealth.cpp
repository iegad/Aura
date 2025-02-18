// Aura copied by iegad


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float
UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluationParameters;
	evaluationParameters.SourceTags = sourceTags;
	evaluationParameters.TargetTags = targetTags;

	float vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, evaluationParameters, vigor);
	vigor = FMath::Max<float>(vigor, 0.f);

	auto* combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetPlayerLevel();

	return 80.f + 2.5f * vigor + 10.f * playerLevel;
}
