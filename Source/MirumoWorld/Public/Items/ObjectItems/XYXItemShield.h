// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/XYXItemBase.h"
#include "Interfaces/XYXInterfaceItem.h"
#include "XYXItemShield.generated.h"

class AXYXDisplayedItem;

UCLASS()
class MIRUMOWORLD_API UXYXItemShield : public UXYXItemBase,
	public IXYXInterfaceItem
{
	GENERATED_BODY()

public:

	UXYXItemShield();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float BlockValue = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TSubclassOf<AXYXDisplayedItem> DisplayedItem;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		float GetBlockValue();
	virtual float GetBlockValue_Implementation() override {
		return BlockValue;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		TSubclassOf<AXYXDisplayedItem>  GetDisplayedItem();
	virtual TSubclassOf<AXYXDisplayedItem> GetDisplayedItem_Implementation() override {
		return DisplayedItem;
	}
	
};
