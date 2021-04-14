// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "XYXInterfaceItemCanBeTwoHanded.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UXYXInterfaceItemCanBeTwoHanded : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MIRUMOWORLD_API IXYXInterfaceItemCanBeTwoHanded
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool IsTwoHanded();
};
