// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANRotateOwner.h"
#include "Components/XYXRotatingComponent.h"

void UXYXANRotateOwner::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			auto RotatingComp = CharacterOwner->FindComponentByClass<UXYXRotatingComponent>();
			if (RotatingComp)
			{
				RotatingComp->StartRotatingWithLimit(MaxPossibleRotation, MaxDegreesPerSecond);
			}
		}
	}
}
