// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DisplayedItems/XYXDisplayedItem.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include <Particles/ParticleSystemComponent.h>
#include "Actors/XYXCharacter.h"



// Sets default values
AXYXDisplayedItem::AXYXDisplayedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void AXYXDisplayedItem::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* MeshComp = GetPrimaryComponent();
	if (IsValid(MeshComp))
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

UPrimitiveComponent* AXYXDisplayedItem::GetPrimaryComponent()
{
	UPrimitiveComponent* Component = nullptr;

	auto StaticMeshComp = Cast<UStaticMeshComponent>(
		this->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (IsValid(StaticMeshComp))
	{
		Component = StaticMeshComp;
	}
	else
	{
		auto SkeletalMeshComp = Cast<USkeletalMeshComponent>(
			this->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		if (IsValid(SkeletalMeshComp))
		{
			Component = SkeletalMeshComp;
		}
		else
		{
			auto ParticleSystemComp = Cast<UParticleSystemComponent>(
				this->GetComponentByClass(UParticleSystemComponent::StaticClass()));
			if (IsValid(ParticleSystemComp))
			{
				Component = ParticleSystemComp;
			}
		}
	}

	return Component;
}

bool AXYXDisplayedItem::Attach()
{
	UPrimitiveComponent* MeshComp = GetPrimaryComponent();
	if (IsValid(MeshComp))
	{
		AXYXCharacter* Character = Cast<AXYXCharacter>(GetOwner());
		if (Character)
		{
			MeshComp->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, GetAttachmentSocket());
			return true;
		}
	}
	
	return false;
}

FName AXYXDisplayedItem::GetAttachmentSocket()
{
	return AttachmentSocket;
}

void AXYXDisplayedItem::SimulatePhysics()
{
	auto Comp = GetPrimaryComponent();
	if (Comp)
	{
		Comp->SetCollisionProfileName(TEXT("Ragdoll"), true);
		Comp->SetSimulatePhysics(true);
	}
}

void AXYXDisplayedItem::SetEquipmemtComp(UXYXEquipmentManagerComponent* Comp)
{
	EquipmentComp = Comp;
}

void AXYXDisplayedItem::SetSlotIndex(int32 Index)
{
	SlotIndex = Index;
}

void AXYXDisplayedItem::SetItemType(EItemType Ty)
{
	ItemType = Ty;
}

