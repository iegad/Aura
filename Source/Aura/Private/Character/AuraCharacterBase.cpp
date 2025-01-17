// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	this->Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	this->Weapon->SetupAttachment(this->GetMesh(), FName(TEXT("WeaponHandSocket")));
	this->Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void 
AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}