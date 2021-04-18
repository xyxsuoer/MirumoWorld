// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/XYXItemBase.h"
#include "Game/XYXData.h"
#include "XYXItemWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MIRUMOWORLD_API UXYXItemWeapon : public UXYXItemBase
{
	GENERATED_BODY()

public:

	UXYXItemWeapon();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		EWeaponType WeaponType = EWeaponType::ENone;
	
};
