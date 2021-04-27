// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANDisplayedItemAttach.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Actors/XYXCharacter.h"

void UXYXANDisplayedItemAttach::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		AXYXCharacter* CharacterOwner = Cast<AXYXCharacter>(MeshComp->GetOwner());
		if (CharacterOwner)
		{
			UXYXEquipmentManagerComponent* EquipmentComp = CharacterOwner->GetEquipmentManagerComponent();
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
