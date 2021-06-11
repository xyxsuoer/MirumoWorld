// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANSHitBox.h"
#include "Components/XYXCollisionHandlerComponent.h"

void UXYXANSHitBox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			auto CollisionHandler = CharacterOwner->FindComponentByClass<UXYXCollisionHandlerComponent>();
			if (CollisionHandler)
			{
				CollisionHandler->ActiveCollision(CollisionPart);
			}
		}
	}
}

void UXYXANSHitBox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			auto CollisionHandler = CharacterOwner->FindComponentByClass<UXYXCollisionHandlerComponent>();
			if (CollisionHandler)
			{
				CollisionHandler->DeactiveCollision();
			}
		}
	}
}
