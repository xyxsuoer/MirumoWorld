// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "XYXANBowActionShootProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXANBowActionShootProjectile : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;
	
};
