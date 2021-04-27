// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "XYXANDisplayedItemAttach.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXANDisplayedItemAttach : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bSlotHiddenChanged = true;

};
