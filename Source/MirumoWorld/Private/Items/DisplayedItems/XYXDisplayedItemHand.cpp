// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DisplayedItems/XYXDisplayedItemHand.h"
#include "Components/XYXEquipmentManagerComponent.h"

AXYXDisplayedItemHand::AXYXDisplayedItemHand()
{

}

void AXYXDisplayedItemHand::BeginPlay()
{
	Super::BeginPlay();
	if (EquipmentComp)
	{
		EquipmentComp->OnSlotHiddenChanged.AddDynamic(this, &AXYXDisplayedItemHand::OnSlotHiddenChanged);
	}
}

FName AXYXDisplayedItemHand::GetAttachmentSocket()
{
	//Super::GetAttachmentSocket();
	if (EquipmentComp)
	{
		if (EquipmentComp->IsSlotHidden(DIItemType, DISlotIndex))
		{
			return DIAttachmentSocket;
		}
		else
		{
			if (EquipmentComp->GetIsInCombat())
			{
				return HandAttachmentSocket;
			}
			else
			{
				return DIAttachmentSocket;
			}
		}
	}

	return DIAttachmentSocket;
}

void AXYXDisplayedItemHand::OnSlotHiddenChanged(EItemType SlotType, int32 SlotIndex, FStoredItem ActiveItem, bool bIsHidden)
{
	if (EquipmentComp && EquipmentComp->GetIsInCombat())
	{
		if (SlotType == DIItemType && SlotIndex == DISlotIndex)
		{
			Attach();
		}
	}
}
