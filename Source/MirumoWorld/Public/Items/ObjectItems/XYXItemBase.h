// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Game/XYXData.h"
#include "XYXItemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MIRUMOWORLD_API UXYXItemBase : public UObject
{
	GENERATED_BODY()

public:

	UXYXItemBase();

	UFUNCTION(BlueprintCallable, Category = XYX)
		virtual void UseItem(AActor* Caller);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		FItem Item;

};
