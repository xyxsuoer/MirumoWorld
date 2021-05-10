// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DisplayedItems/XYXDisplayedItemQuiver.h"
#include "Components/SceneComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Components/XYXEquipmentManagerComponent.h>
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Items/ObjectItems/XYXItemArrow.h>


AXYXDisplayedItemQuiver::AXYXDisplayedItemQuiver()
{
	QuiverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	QuiverMesh->SetupAttachment(SceneComponent);

	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh Comp"));
	InstancedStaticMesh->SetupAttachment(QuiverMesh);

	for (int32 i = 0; i < 9; ++i)
	{
		FString TmpText = TEXT("Arrow Spawn") + FString::FromInt(i);
		auto ArrowSpawn = CreateDefaultSubobject<USceneComponent>(*TmpText);
		ArrowSpawn->SetupAttachment(QuiverMesh);
		ArrowSpawns.Emplace(ArrowSpawn);
	}
}

void AXYXDisplayedItemQuiver::BeginPlay()
{
	Super::BeginPlay();

	UpdateArrowMesh();
}

void AXYXDisplayedItemQuiver::UpdateArrowMesh()
{
	if (EquipmentComp)
	{
		FStoredItem TmpItem = EquipmentComp->GetActiveItem(EItemType::EArrows, 0);
		TSubclassOf<class UXYXItemBase> ItemArrowClass = TmpItem.ItemClass;
		if (UKismetSystemLibrary::IsValidClass(ItemArrowClass))
		{
			auto ItemArrow = NewObject<UXYXItemArrow>(this, ItemArrowClass);
			if (ItemArrow)
			{
				InstancedStaticMesh->SetStaticMesh(ItemArrow->ArrowMesh);
			}
		}
	}
}

void AXYXDisplayedItemQuiver::UpdateQuiver()
{
	if (EquipmentComp && InstancedStaticMesh)
	{
		LastUpdatedArrow = EquipmentComp->GetActiveItem(EItemType::EArrows, 0);
		InstancedStaticMesh->ClearInstances();
		int32 LastIndex = LastUpdatedArrow.Amount - 1; // 有一把箭在手上不在箭袋里
		for (int32 i = 1; i <= LastIndex; ++i)
		{
			if (i > ArrowSpawns.Num())
			{
				break;
			}
		
			if (ArrowSpawns.IsValidIndex(i - 1))
			{
				InstancedStaticMesh->AddInstance(ArrowSpawns[i - 1]->GetRelativeTransform());
			}
		}
	}
}

void AXYXDisplayedItemQuiver::SetEquipmemtComp(class UXYXEquipmentManagerComponent* Comp)
{
	EquipmentComp = Comp;

	if (EquipmentComp)
	{
		EquipmentComp->OnActiveItemChanged.AddDynamic(this, &AXYXDisplayedItemQuiver::HandleOnActiveItemChanged);
		EquipmentComp->OnSlotHiddenChanged.AddDynamic(this, &AXYXDisplayedItemQuiver::HandleOnSlotHiddenChanged);
	}
}

void AXYXDisplayedItemQuiver::HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex)
{
	if (Type == EItemType::EArrows && NewItem.Amount > LastUpdatedArrow.Amount)
	{
		UpdateQuiver();
	}
}

void AXYXDisplayedItemQuiver::HandleOnSlotHiddenChanged(EItemType SlotType, int32 SlotIndex, FStoredItem ActiveItem, bool bIsHidden)
{
	if (EquipmentComp && EquipmentComp->GetIsInCombat())
	{
		if (SlotType == DIItemType && SlotIndex == DISlotIndex)
		{
			Attach();
		}
	}
}
