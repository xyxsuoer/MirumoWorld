// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANSIgnoreRootMotion.h"
#include "Actors/XYXCharacter.h"

void UXYXANSIgnoreRootMotion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetAnimInstance())
	{
		MeshComp->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}
}

void UXYXANSIgnoreRootMotion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetAnimInstance())
	{
		MeshComp->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	}
}