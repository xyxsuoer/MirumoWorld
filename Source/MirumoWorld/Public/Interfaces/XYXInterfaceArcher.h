// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "XYXInterfaceArcher.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UXYXInterfaceArcher : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MIRUMOWORLD_API IXYXInterfaceArcher
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		float GetAimAlpha();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool DoesHoldBowString();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		FName GetBowStringSocketName();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		void ShootArrowProjectile();
};
