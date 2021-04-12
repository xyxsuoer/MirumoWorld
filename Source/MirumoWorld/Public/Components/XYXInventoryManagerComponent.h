// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/XYXData.h"
#include "XYXInventoryManagerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, FStoredItem, AddedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, FStoredItem, RemovedItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXInventoryManagerComponent();

	UPROPERTY(BlueprintAssignable)
		FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable)
		FOnItemRemoved OnItemRemoved;

	UFUNCTION(BlueprintCallable, Category = XYX)
		void AddItem(TSubclassOf<class UXYXItemBase> ItemClass, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void RemoveItem(TSubclassOf<class UXYXItemBase> ItemClass, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void ClearInventory();

	UFUNCTION(BlueprintCallable, Category = XYX)
		void RemoveItemAtIndex(int32 Index, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void DropItem(FStoredItem Item);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void UseItem(FGuid ItemId);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsSlotEmpty(int32 Index);

	UFUNCTION(BlueprintCallable, Category = XYX)
		FStoredItem GetItemAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = XYX)
		int32 FindIndexByClass(TSubclassOf<class UXYXItemBase> ItemClass);

	UFUNCTION(BlueprintCallable, Category = XYX)
		int32 FindIndexById(FGuid Id);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsItemValid(FStoredItem Item);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FStoredItem> Inventory;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class AXYXCharacter* CharacterOwner;
};
