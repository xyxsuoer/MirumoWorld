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
	
	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static class UXYXExtendedStatComponent* GetExtendedStatComp(AActor* Actor, EAttributesType Type);

	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static float ScaleMeleeDamageByType(float Damage, EMeleeAttackType MeleeAttackType);

	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static float ScaleMeleeAttackStaminaCostByType(float Cost, EMeleeAttackType MeleeAttackType);

	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static EDirection GetHitDirection(FVector HitFromDirection, AActor* AttackedActor);

	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static void PlayHitSound(UXYXGameInstance* GameInstance, AActor* ActorWhoHit, AActor* ActorWhoWasHit, FVector Location);

	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static void PlayParrySound(UXYXGameInstance* GameInstance, AActor* ActorWhoParried, AActor* ActorWhoWasParried, FVector Location);

	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static void PlayBlockSound(UXYXGameInstance* GameInstance, AActor* ActorWhoBlocked, AActor* ActorWhoWasBlocked, FVector Location);

	UFUNCTION(BlueprintCallable, Category = XYXLibrary)
		static AActor* GetClosestActor(AActor* Target, TArray<AActor*> Actors);

};
