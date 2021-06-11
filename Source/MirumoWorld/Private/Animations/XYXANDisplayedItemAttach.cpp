// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANDisplayedItemAttach.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Actors/XYXCharacter.h"

void UXYXANDisplayedItemAttach::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			UXYXEquipmentManagerComponent* EquipmentComp = CharacterOwner->FindComponentByClass<UXYXEquipmentManagerComponent>();
			if (EquipmentComp)
			{
				if (bSlotHiddenChanged)
				{
					EquipmentComp->SlotHiddenChangedBroadcast();
				}
				else
				{
					EquipmentComp->DelayUpdateDisplayedItem();
				}
			}
		}
	}
}
