// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANSInputBuffer.h"
#include "Actors/XYXCharacter.h"
#include "Components/XYXInputBufferComponent.h"

void UXYXANSInputBuffer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			UXYXInputBufferComponent* InputBuffer = CharacterOwner->FindComponentByClass<UXYXInputBufferComponent>();
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
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			UXYXInputBufferComponent* InputBuffer = CharacterOwner->FindComponentByClass<UXYXInputBufferComponent>();
			if (InputBuffer)
			{
				InputBuffer->CloseInputBuffer();
			}
		}
	}
}
