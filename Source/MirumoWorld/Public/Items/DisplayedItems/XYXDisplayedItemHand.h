// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/DisplayedItems/XYXDisplayedItem.h"
#include "XYXDisplayedItemHand.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API AXYXDisplayedItemHand : public AXYXDisplayedItem
{
	GENERATED_BODY()

public:

	AXYXDisplayedItemHand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FName HandAttachmentSocket = TEXT("None");

	virtual	FName GetAttachmentSocket() override;

	virtual void SetEquipmemtComp(class UXYXEquipmentManagerComponent* Comp) override;

	UFUNCTION()
		void HandleOnSlotHiddenChanged(EItemType SlotType, int32 SlotIndex, FStoredItem ActiveItem, bool bIsHidden);
	
};
