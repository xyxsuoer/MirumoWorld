// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/XYXItemBase.h"
#include "Interfaces/XYXInterfaceItem.h"
#include "XYXItemArrow.generated.h"

struct FModifier;
class AXYXDisplayedItem;

UCLASS()
class MIRUMOWORLD_API UXYXItemArrow : public UXYXItemBase,
	public IXYXInterfaceItem
{
	GENERATED_BODY()

public:

	UXYXItemArrow();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UStaticMesh* ArrowMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TSubclassOf<AXYXDisplayedItem> DisplayedItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TArray<FModifier> Modifiers;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		TSubclassOf<AXYXDisplayedItem>  GetDisplayedItem();
	virtual TSubclassOf<AXYXDisplayedItem> GetDisplayedItem_Implementation() override {
		return DisplayedItem;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		TArray<FModifier> GetModifiers();
	virtual TArray<FModifier> GetModifiers_Implementation() override {
		return Modifiers;
	}
	
};
