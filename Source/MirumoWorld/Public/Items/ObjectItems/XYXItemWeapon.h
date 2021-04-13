// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/XYXItemBase.h"
#include "Game/XYXData.h"
#include "XYXItemWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXItemWeapon : public UXYXItemBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EWeaponType WeaponType;
	
};
