// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Game/XYXData.h"
#include "XYXANSHitBox.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXANSHitBox : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		ECollisionPart CollisionPart = ECollisionPart::EMainHandItem;
	
};
