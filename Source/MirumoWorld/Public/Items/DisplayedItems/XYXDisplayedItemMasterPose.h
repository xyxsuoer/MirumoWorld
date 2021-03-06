// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/DisplayedItems/XYXDisplayedItem.h"
#include "XYXDisplayedItemMasterPose.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API AXYXDisplayedItemMasterPose : public AXYXDisplayedItem
{
	GENERATED_BODY()

public:

	AXYXDisplayedItemMasterPose();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual bool Attach() override;
	
};
