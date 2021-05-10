// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DisplayedItems/XYXDisplayedItemBow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Actors/XYXCharacter.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include <Items/ObjectItems/XYXItemArrow.h>
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

AXYXDisplayedItemBow::AXYXDisplayedItemBow()
{
	BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Comp"));
	BowMesh->SetupAttachment(SceneComponent);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	ArrowMesh->AttachToComponent(BowMesh, FAttachmentTransformRules::KeepRelativeTransform, ArrowSocketName);
}

void AXYXDisplayedItemBow::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateArrowMesh();
}

UPrimitiveComponent* AXYXDisplayedItemBow::GetPrimaryComponent()
{
	UPrimitiveComponent* Component = nullptr;

	if (IsValid(BowMesh))
		Component = BowMesh;

	return Component;
}

bool AXYXDisplayedItemBow::Attach()
{
	bool Result = false;
	if (!EquipmentComp)
	{
		return Result;
	}

	if (EquipmentComp->GetIsInCombat() && EquipmentComp->GetCombatType() == ECombatType::ERanged)
	{
		UpdateArrowVisibility(true);
	}
	else
	{
		UpdateArrowVisibility(false);
	}

	UpdateArrowAttachment(false);

	Result = Super::Attach();
	return Result;
}

void AXYXDisplayedItemBow::SetEquipmemtComp(class UXYXEquipmentManagerComponent* Comp)
{
	EquipmentComp = Comp;

	if (EquipmentComp)
	{
		EquipmentComp->OnActiveItemChanged.AddDynamic(this, &AXYXDisplayedItemBow::HandleOnActiveItemChanged);
		EquipmentComp->OnSlotHiddenChanged.AddDynamic(this, &AXYXDisplayedItemBow::HandleOnSlotHiddenChanged);
	}
}

void AXYXDisplayedItemBow::SimulatePhysics()
{
	Super::SimulatePhysics();

	if (IsValid(ArrowMesh))
	{
		ArrowMesh->SetCollisionProfileName(TEXT("Ragdoll"), true);
		ArrowMesh->SetSimulatePhysics(true);
	}
}

void AXYXDisplayedItemBow::UpdateArrowVisibility(bool bVisible)
{
	if (IsValid(ArrowMesh))
	{
		ArrowMesh->SetVisibility(bVisible, false);
	}
}

void AXYXDisplayedItemBow::UpdateArrowAttachment(bool bAttachToOwner)
{
	if (bAttachToOwner)
	{
		AXYXCharacter* Character = Cast<AXYXCharacter>(GetOwner());
		if (Character && IsValid(ArrowMesh))
		{
			FName TmpName = Character->GetBowStringSocketName();
			ArrowMesh->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TmpName);
		}
	}
	else
	{
		if (IsValid(ArrowMesh) && IsValid(BowMesh))
		{
			ArrowMesh->AttachToComponent(BowMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ArrowSocketName);
		}
	}
}

void AXYXDisplayedItemBow::UpdateArrowMesh()
{
	if (EquipmentComp)
	{
		LastUpdatedArrow = EquipmentComp->GetActiveItem(EItemType::EArrows, 0);
		TSubclassOf<class UXYXItemBase> ItemArrowClass = LastUpdatedArrow.ItemClass;
		if (UKismetSystemLibrary::IsValidClass(ItemArrowClass))
		{
			auto ItemArrow = NewObject<UXYXItemArrow>(this, ItemArrowClass);
			if (ItemArrow)
			{
				ArrowMesh->SetStaticMesh(ItemArrow->ArrowMesh);
				return;
			}
		}
		
		ArrowMesh->SetStaticMesh(nullptr);	
	}
}

void AXYXDisplayedItemBow::HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex)
{
	if (Type == EItemType::EArrows && NewItem.Id != LastUpdatedArrow.Id )
	{
		UpdateArrowMesh();
	}
}
