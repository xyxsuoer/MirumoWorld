// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "XYXGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameSaved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLoaded);

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API AXYXGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AXYXGameMode();

	UPROPERTY(BlueprintAssignable)
	FOnGameSaved OnGameSaved;

	UPROPERTY(BlueprintAssignable)
	FOnGameLoaded OnGameLoaded;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class UXYXSaveGame* XYXSaveGame;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FString SaveGameName = "Save_XYX";

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
