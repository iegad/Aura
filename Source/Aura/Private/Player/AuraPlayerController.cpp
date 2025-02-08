#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	this->bReplicates = true;
}

void 
AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void 
AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	auto* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (subsystem) {
		subsystem->AddMappingContext(AuraContext, 0);
	}

	// 显示鼠标
	bShowMouseCursor = true;

	// 默认鼠标样式
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI inputModeData;

	// 设置视窗的鼠标锁定行为
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	// 是否在鼠标按下导致的临时鼠标捕获期间隐藏光标
	inputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(inputModeData);
}

void 
AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	auto* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
}

void 
AAuraPlayerController::Move(const FInputActionValue& InputActionValue) 
{
	const FVector2D inputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator rotation = GetControlRotation();
	const FRotator yawRotation(0.f, rotation.Yaw, 0.f);

	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	APawn* controllerPawn = GetPawn<APawn>();
	if (IsValid(controllerPawn)) {
		controllerPawn->AddMovementInput(forwardDirection, inputAxisVector.Y);
		controllerPawn->AddMovementInput(rightDirection, inputAxisVector.X);
	}
}

void 
AAuraPlayerController::CursorTrace()
{
	FHitResult cursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
	if (!cursorHit.bBlockingHit) {
		return;
	}

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(cursorHit.GetActor());

	// 鼠标的射线追踪, 会出现多种情况:
	//  1, LastActor == NULL && ThisActor == NULL
	//		- 什么都不作
	//  2, LastActor == NULL && ThisActor != NULL
	//		- ThisActor高亮
	//  3, LastActor != NULL && ThisActor == NULL
	//		- LastActor 取消高亮
	//  4, LastActor != NULL && ThisActor != NULL && ThisActor != LastActor
	//		- ThisActor 高亮, LastActor 取消高亮
	//	4, LastActor != NULL && ThisActor != NULL && ThisActor == LastActor
	//		- 什么都不作

	if (!LastActor) {
		if (!ThisActor) {
			// Case 1
		}
		else {
			// Case 2
			ThisActor->HighlightActor();
		}
	}
	else {
		if (!ThisActor) {
			// Case 3
			LastActor->UnHightlightActor();
		}
		else {
			if (ThisActor != LastActor) {
				// Case 4
				LastActor->UnHightlightActor();
				ThisActor->HighlightActor();
			}
			else {
				// Case 5
			}
		}
	}
}
