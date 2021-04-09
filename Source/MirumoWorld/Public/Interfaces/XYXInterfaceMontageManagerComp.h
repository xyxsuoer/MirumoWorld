// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Game/XYXData.h"
#include "XYXInterfaceMontageManagerComp.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UXYXInterfaceMontageManagerComp : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MIRUMOWORLD_API IXYXInterfaceMontageManagerComp
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		UDataTable* GetMontages(EMontageAction Action);
};
