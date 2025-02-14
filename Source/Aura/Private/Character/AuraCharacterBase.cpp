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
AAuraCharacterBase::InitializePrimaryAttributes() const
{
	auto* asc = GetAbilitySystemComponent();
	check(asc && DefaultPrimaryAttributes);

	auto contextHandle = asc->MakeEffectContext();
	auto specHandle = asc->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, contextHandle);
	asc->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), asc);
}
