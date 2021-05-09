// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Game/XYXData.h"
#include "XYXInterfaceEntity.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UXYXInterfaceEntity : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MIRUMOWORLD_API IXYXInterfaceEntity
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		ETeam GetEntityCombatTeam();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool IsEntityAlive();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		FName GetHeadSocket();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool TakeDamage(FHitData HitData, EAttackResult& ResultType);

};
