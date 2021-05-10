// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XYXData.h"
#include "XYXFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static EMontageAction ConvertMeleeAttackTypeToAction(const EMeleeAttackType Type);

	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static float GetCrosshairDistanceLocation();
};
