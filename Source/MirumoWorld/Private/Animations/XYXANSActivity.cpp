// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANSActivity.h"
#include "Components/XYXStateManagerComponent.h"

void UXYXANSActivity::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			auto StateManagerComp = CharacterOwner->FindComponentByClass<UXYXStateManagerComponent>();
			if (StateManagerComp)
			{
				StateManagerComp->SetActivity(InActivity, true);
			}
		}
	}
}

void UXYXANSActivity::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			auto StateManagerComp = CharacterOwner->FindComponentByClass<UXYXStateManagerComponent>();
			if (StateManagerComp)
			{
				StateManagerComp->SetActivity(InActivity, false);
			}
		}
	}
}
