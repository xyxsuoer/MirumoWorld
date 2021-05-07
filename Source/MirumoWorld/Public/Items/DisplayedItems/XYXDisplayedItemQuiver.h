// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/DisplayedItems/XYXDisplayedItem.h"
#include "XYXDisplayedItemQuiver.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API AXYXDisplayedItemQuiver : public AXYXDisplayedItem
{
	GENERATED_BODY()

public:

	AXYXDisplayedItemQuiver();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		struct FStoredItem LastUpdatedArrow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UStaticMeshComponent* QuiverMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UInstancedStaticMeshComponent* InstancedStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = XYX)
		TArray<USceneComponent* > ArrowSpawns = {};

	virtual void SetEquipmemtComp(class UXYXEquipmentManagerComponent* Comp) override;

	UFUNCTION()
		void UpdateArrowMesh();

	UFUNCTION()
		void UpdateQuiver();

	UFUNCTION()
		void HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex);
};
