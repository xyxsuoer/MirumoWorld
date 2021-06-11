// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANSRotateOwner.h"
#include "Components/XYXRotatingComponent.h"

void UXYXANSRotateOwner::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			auto InputBuffer = CharacterOwner->FindComponentByClass<UXYXRotatingComponent>();
			if (InputBuffer)
			{
				InputBuffer->StartRotatingWithTime(10.f, MaxDegreesPerSecond);
			}
		}
	}
}

void UXYXANSRotateOwner::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			auto InputBuffer = CharacterOwner->FindComponentByClass<UXYXRotatingComponent>();
			if (InputBuffer)
			{
				InputBuffer->StopRotating();
			}
		}
	}
}
