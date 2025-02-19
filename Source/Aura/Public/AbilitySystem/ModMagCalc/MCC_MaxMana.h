// Aura copied by iegad

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MCC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMCC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMCC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// 定义捕获数据的结构体
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
