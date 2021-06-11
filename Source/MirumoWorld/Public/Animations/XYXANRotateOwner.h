// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "XYXANRotateOwner.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXANRotateOwner : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float MaxDegreesPerSecond = 720.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float MaxPossibleRotation = 180.f;
};
