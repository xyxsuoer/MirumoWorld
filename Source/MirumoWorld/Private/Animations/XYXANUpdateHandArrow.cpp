// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANUpdateHandArrow.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Actors/XYXCharacter.h"
#include "Items/DisplayedItems/XYXDisplayedItemBow.h"

void UXYXANUpdateHandArrow::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			UXYXEquipmentManagerComponent* EquipmentComp = CharacterOwner->FindComponentByClass<UXYXEquipmentManagerComponent>();
			if (EquipmentComp)
			{
				AXYXDisplayedItem* DIOne = nullptr;
				AXYXDisplayedItem* DISecond = nullptr;
				EquipmentComp->GetDisplayedItem(EItemType::ERangeWeapon, 0, DIOne, DISecond);
				if (IsValid(DIOne))
				{
					AXYXDisplayedItemBow* DIBow = Cast<AXYXDisplayedItemBow>(DIOne);
					if (DIBow)
					{
						DIBow->UpdateArrowAttachment(bAttachToHand);
						DIBow->UpdateArrowVisibility(bVisible);
					}
				}
			}
		}
	}
}