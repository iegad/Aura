#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(this->GetMesh(), FName(TEXT("WeaponHandSocket")));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* 
AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void
AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void
AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const
{
	auto* asc = GetAbilitySystemComponent();
	check(asc && GameplayEffect);

	auto contextHandle = asc->MakeEffectContext();
	contextHandle.AddSourceObject(this);

	const auto specHandle = asc->MakeOutgoingSpec(GameplayEffect, Level, contextHandle);
	asc->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), asc);
}

void
AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}