// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "XYXData.h"
#include "XYXSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UXYXSaveGame();

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FStoredItem> Inventory = {};

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FEquipmentSlots> EquipmentSlots = {};

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsInCombat = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EItemType SelectMainHandSlotType = EItemType::ENone;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TMap<EAttributesType, float> StatValues = {};
	
};
