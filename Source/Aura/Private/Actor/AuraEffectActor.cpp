#include "Actor/AuraEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void 
AAuraEffectActor::OnBeginOverlap(UPrimitiveComponent* OverplappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherObdyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: 暂时先使用 const_cast 来修改常指针对象的值
	auto* ascInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (!ascInterface) {
		return;
	}

	auto* auraAttributeSet = Cast<UAuraAttributeSet>(ascInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
	auto* todo = const_cast<UAuraAttributeSet*>(auraAttributeSet);
	todo->SetHealth(auraAttributeSet->GetHealth() + 25.f);
}

void
AAuraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void 
AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}