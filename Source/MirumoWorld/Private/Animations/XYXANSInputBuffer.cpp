// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANSInputBuffer.h"
#include "Actors/XYXCharacter.h"
#include "Components/XYXInputBufferComponent.h"

void UXYXANSInputBuffer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		AXYXCharacter* CharacterOwner = Cast<AXYXCharacter>(MeshComp->GetOwner());
		if (CharacterOwner)
		{
			UXYXInputBufferComponent* InputBuffer = CharacterOwner->GetInputBufferComponent();
			if (InputBuffer)
			{
				InputBuffer->OpenInputBuffer();
			}
		}
	}
}

void UXYXANSInputBuffer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		AXYXCharacter* CharacterOwner = Cast<AXYXCharacter>(MeshComp->GetOwner());
		if (CharacterOwner)
		{
			UXYXInputBufferComponent* InputBuffer = CharacterOwner->GetInputBufferComponent();
			if (InputBuffer)
			{
				InputBuffer->CloseInputBuffer();
			}
		}
	}
}
