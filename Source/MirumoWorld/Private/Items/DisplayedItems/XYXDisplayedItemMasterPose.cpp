// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DisplayedItems/XYXDisplayedItemMasterPose.h"
#include "GameFramework/Character.h"

AXYXDisplayedItemMasterPose::AXYXDisplayedItemMasterPose()
{

}

void AXYXDisplayedItemMasterPose::BeginPlay()
{
	Super::BeginPlay();
}

bool AXYXDisplayedItemMasterPose::Attach()
{
	if (Super::Attach())
	{
		auto SkeletaMeshComp = Cast<USkeletalMeshComponent>(GetPrimaryComponent());
		if (SkeletaMeshComp)
		{
			auto Character = Cast<ACharacter>(GetOwner());
			if (Character)
			{
				SkeletaMeshComp->SetMasterPoseComponent(Character->GetMesh(), false);
				return true;
			}
		}
	}

	return false;
}
