// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/XYXData.h"
#include "XYXStatsManagerComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifierAdded, EAttributesType, Type, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifierRemoved, EAttributesType, Type, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBaseValueChanged, EAttributesType, Type, float, NewValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXStatsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXStatsManagerComponent();

	UPROPERTY(BlueprintAssignable)
		FOnModifierAdded OnModifierAdded;

	UPROPERTY(BlueprintAssignable)
		FOnModifierRemoved OnModifierRemoved;

	UPROPERTY(BlueprintAssignable)
		FOnBaseValueChanged OnBaseValueChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXEquipmentManagerComponent* EquipmentComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		TArray<FAttributes> Stats;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float InitialBlockValue = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float RecentlyReceivedDamage = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		int32 RecentlyReceivedHitsCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float RecentlyReceivedSuccessfulDamage = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		int32 RecentlyReceivedSuccessfulHitsCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FTimerHandle ResetRecentlyReceivedDamageTimer;

	UFUNCTION()
		void ExcludeItemModifiers(TSubclassOf<class UXYXItemBase> Item);

	UFUNCTION()
		void IncludeItemModifiers(TSubclassOf<class UXYXItemBase> Item);

	UFUNCTION()
		void AddModifier(EAttributesType Type, float Value);

	UFUNCTION()
		void RemoveModifier(EAttributesType Type, float Value);

	UFUNCTION()
		float GetStatValue(EAttributesType Type, bool bIncludeModifiers);

	UFUNCTION()
		int32 GetStatIndex(EAttributesType Type);

	UFUNCTION()
		float GetDamage();

	UFUNCTION()
		void ChangeStatBaseValue(EAttributesType Type, float Value);

	UFUNCTION()
		void TakeDamage(float Damage, bool bIgnoreStamina);

	UFUNCTION()
		float GetRecentlyReceivedDamage();

	UFUNCTION()
		void ResetRecentlyReceivedDamage();

	UFUNCTION()
		int32 GetRecentlyReceivedHitsCount();

	UFUNCTION()
		int32 GetRecentlyReceivedSuccessfulHitsCount();

	UFUNCTION()
		float GetRecentlyReceivedSuccessfulDamage();

	UFUNCTION()
		void UpdateBlockBaseValue();

	UFUNCTION()
		void Initialize();

	UFUNCTION()
		void HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex);

	UFUNCTION()
		void HandleOnSlotHiddenChanged(EItemType SlotType, int32 SlotIndex, FStoredItem ActiveItem, bool bIsHidden);

	UFUNCTION()
		void HandleOnMainHandTypeChanged(EItemType Type);

};
