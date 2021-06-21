// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANBowActionShootProjectile.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Actors/XYXCharacter.h"
#include "Items/DisplayedItems/XYXDisplayedItemQuiver.h"
#include "Interfaces/XYXInterfaceArcher.h"

void UXYXANBowActionShootProjectile::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		ACharacter* CharacterOwner = Cast<ACharacter>(MeshComp->GetOwner());
		IXYXInterfaceArcher* ArcherCharacter = Cast<IXYXInterfaceArcher>(CharacterOwner);
		if (ArcherCharacter)
		{
			ArcherCharacter->Execute_ShootArrowProjectile(CharacterOwner);
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

