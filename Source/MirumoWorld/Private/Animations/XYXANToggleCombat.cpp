// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANToggleCombat.h"
#include "Actors/XYXCharacter.h"
#include "Components/XYXEquipmentManagerComponent.h"


void UXYXANToggleCombat::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			UXYXEquipmentManagerComponent* EquipmentComp = CharacterOwner->FindComponentByClass<UXYXEquipmentManagerComponent>();
			if (EquipmentComp)
			{
				EquipmentComp->ToggleCombat();
			}
		}
	}
}
