// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/DisplayedItems/XYXDisplayedItemHand.h"
#include "XYXDisplayedItemBow.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API AXYXDisplayedItemBow : public AXYXDisplayedItemHand
{
	GENERATED_BODY()

public:
	AXYXDisplayedItemBow();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FName ArrowSocketName = TEXT("arrow");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		struct FStoredItem LastUpdatedArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class USkeletalMeshComponent* BowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UStaticMeshComponent* ArrowMesh;

public:

	virtual UPrimitiveComponent* GetPrimaryComponent() override;

	virtual bool Attach() override;

	virtual void SetEquipmemtComp(class UXYXEquipmentManagerComponent* Comp) override;

	virtual	void SimulatePhysics() override;

	UFUNCTION()
		void UpdateArrowVisibility(bool bVisible);

	UFUNCTION()
		void UpdateArrowAttachment(bool bAttachToOwner);

	UFUNCTION()
		void UpdateArrowMesh();

	UFUNCTION()
		void HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex);
	
};
