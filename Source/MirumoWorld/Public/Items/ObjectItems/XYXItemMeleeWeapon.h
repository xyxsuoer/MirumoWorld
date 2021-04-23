// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/XYXItemWeapon.h"
#include "Interfaces/XYXInterfaceItem.h"
#include "XYXItemMeleeWeapon.generated.h"


struct FModifier;
class AXYXDisplayedItem;

UCLASS(Blueprintable, BlueprintType)
class MIRUMOWORLD_API UXYXItemMeleeWeapon : public UXYXItemWeapon,
	public IXYXInterfaceItem
{
	GENERATED_BODY()

public:

	UXYXItemMeleeWeapon();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float BlockValue = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		bool bTwoHanded = false;

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
		float GetBlockValue();
	virtual float GetBlockValue_Implementation() override {
		return BlockValue;
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
