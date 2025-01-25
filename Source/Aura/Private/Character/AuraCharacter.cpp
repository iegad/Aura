#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	UCharacterMovementComponent* characterMovement = GetCharacterMovement();
	check(characterMovement);

	characterMovement->bOrientRotationToMovement = true;
	characterMovement->RotationRate = FRotator(0.f, 400.f, 0.f);
	characterMovement->bConstrainToPlane = true;
	characterMovement->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void 
AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 初始化 AbilityActorInfo 服务端
	InitAbilityActorInfo();
}

void 
AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 初始化 AbilityActorInfo 客户端
	InitAbilityActorInfo();
}

void 
AAuraCharacter::InitAbilityActorInfo()
{
	auto* auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);

	auraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(auraPlayerState, this);
	AbilitySystemComponent = auraPlayerState->GetAbilitySystemComponent();
	AttributeSet = auraPlayerState->GetAttributeSet();

	auto* auraPlayerController = Cast<AAuraPlayerController>(GetController());
	if (!auraPlayerController) {
		return;
	}

	auto* auraHUD = Cast<AAuraHUD>(auraPlayerController->GetHUD());
	if (!auraHUD) {
		return;
	}

	auraHUD->InitOverlay(auraPlayerController, auraPlayerState, AbilitySystemComponent, AttributeSet);
}
