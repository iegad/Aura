// Aura copied by iegad


#include "AbilitySystem/ModMagCalc/MCC_MaxMana.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include <Interaction/CombatInterface.h>


UMCC_MaxMana::UMCC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float
UMCC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluationParameters;
	evaluationParameters.SourceTags = sourceTags;
	evaluationParameters.TargetTags = targetTags;

	float intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, evaluationParameters, intelligence);
	intelligence = FMath::Max<float>(intelligence, 0.f);

	auto* combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetPlayerLevel();

	return 50.f + 2.5f * intelligence + 15.f * playerLevel;
}
