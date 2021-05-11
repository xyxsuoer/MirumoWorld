// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANBowActionShootProjectile.h"
#include "Actors/XYXCharacter.h"

void UXYXANBowActionShootProjectile::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		AXYXCharacter* CharacterOwner = Cast<AXYXCharacter>(MeshComp->GetOwner());
		if (CharacterOwner)
		{
			CharacterOwner->ShootArrowProjectile();
		}
	}
}

