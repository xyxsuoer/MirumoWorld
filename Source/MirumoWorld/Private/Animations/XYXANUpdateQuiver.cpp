// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANUpdateQuiver.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Actors/XYXCharacter.h"
#include "Items/DisplayedItems/XYXDisplayedItemQuiver.h"

void UXYXANUpdateQuiver::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
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
				EquipmentComp->GetDisplayedItem(EItemType::EArrows, 0, DIOne, DISecond);
				if (IsValid(DIOne))
				{
					AXYXDisplayedItemQuiver* DIQuiver = Cast<AXYXDisplayedItemQuiver>(DIOne);
					if (DIQuiver)
					{
						DIQuiver->UpdateQuiver();
					}
				}
			}
		}
	}
}