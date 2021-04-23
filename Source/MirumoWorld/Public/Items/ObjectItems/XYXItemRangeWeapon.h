// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/XYXItemWeapon.h"
#include "Interfaces/XYXInterfaceItem.h"
#include "XYXItemRangeWeapon.generated.h"

class AXYXDisplayedItem;
struct FModifier;

UCLASS(Blueprintable, BlueprintType)
class MIRUMOWORLD_API UXYXItemRangeWeapon : public UXYXItemWeapon,
	public IXYXInterfaceItem
{
	GENERATED_BODY()

public:

	UXYXItemRangeWeapon();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		bool bTwoHanded = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TSubclassOf<AXYXDisplayedItem> DisplayedItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TArray<FModifier> Modifiers;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool IsTwoHanded();
	virtual bool IsTwoHanded_Implementation() override {
		return bTwoHanded;
	}

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
