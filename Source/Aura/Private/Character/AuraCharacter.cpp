#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	auto* characterMovement = GetCharacterMovement();
	check(characterMovement);

	characterMovement->bOrientRotationToMovement = true;			// 运动转向移动方向
	characterMovement->RotationRate = FRotator(0.f, 400.f, 0.f);	// Yaw轴旋转速度
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
	UE_LOG(LogTemp, Warning, TEXT("Call in PossessedBy"));
	InitAbilityActorInfo();
}

void 
AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 初始化 AbilityActorInfo 客户端
	UE_LOG(LogTemp, Warning, TEXT("Call in OnRep_PlayerState"));
	InitAbilityActorInfo();
}

void 
AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Call in BeginPlay"));
}

void 
AAuraCharacter::InitAbilityActorInfo()
{
	auto* auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);

	auto* auraAbilitySystemComp = Cast<UAuraAbilitySystemComponent>(auraPlayerState->GetAbilitySystemComponent());
	auraAbilitySystemComp->InitAbilityActorInfo(auraPlayerState, this);
	auraAbilitySystemComp->AbilityActorInfoSet();

	AbilitySystemComponent = auraAbilitySystemComp;
	AttributeSet = auraPlayerState->GetAttributeSet();

	InitializePrimaryAttributes();

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
