// Aura copied by iegad


#include "AbilitySystem/ModMagCalc/MCC_MaxMana.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include <Interaction/CombatInterface.h>


UMCC_MaxMana::UMCC_MaxMana()
{
	// 需要捕获的是 Intelligence 属性
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();	
	// 属性源, 只有两个 GE本身所属目标或是应用目标, 这里设置为Target 表示 MCC所在GE的应用对象, 即AuraCharacter
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// 快照是用来回档的
	IntelligenceDef.bSnapshot = false;

	// 要捕获的与计算相关的属性
	//  添加到该数组中, IntelligenceDef将会被得理
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float
UMCC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedSourceTags.GetAggregatedTags();

	// 用于计算的参数结构
	FAggregatorEvaluateParameters evaluationParameters;
	evaluationParameters.SourceTags = sourceTags;
	evaluationParameters.TargetTags = targetTags;

	float intelligence = 0.f;

	// 在GE被应用获取 intelligence 值
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, evaluationParameters, intelligence);
	intelligence = FMath::Max(intelligence, 0.f);

	// 获取角色等级
	auto* combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetPlayerLevel();

	return 50.f + 2.5f * intelligence + 15.f * playerLevel;
}
