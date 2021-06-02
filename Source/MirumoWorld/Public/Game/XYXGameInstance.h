// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "XYXGameInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MIRUMOWORLD_API UXYXGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UXYXGameInstance();

	UPROPERTY(BlueprintReadWrite, Category = "Levels")
		TArray<FName> LoadedLevels;
	UPROPERTY(BlueprintReadWrite, Category = "Levels")
		TArray<FName> UnloadedLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = XYX)
		TMap<FString, UDataTable*> MontageDataTables;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = XYX)
		UDataTable* SoundDataTable;

};
