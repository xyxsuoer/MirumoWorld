// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DisplayedItems/XYXDisplayedItem.h"
#include "Components/XYXEquipmentManagerComponent.h"



// Sets default values
AXYXDisplayedItem::AXYXDisplayedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AXYXDisplayedItem::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AXYXDisplayedItem::Attach()
{
	return true;
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

