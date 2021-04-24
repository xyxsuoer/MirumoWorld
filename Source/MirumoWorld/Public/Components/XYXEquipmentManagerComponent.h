// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/XYXData.h"
#include "XYXEquipmentManagerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInCombatChanged, bool, bIsInCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMainHandTypeChanged, EItemType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatTypeChanged, ECombatType, CombatType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponTypeChanged, EWeaponType, WeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnItemInSlotChanged, FStoredItem, OldItem, FStoredItem, NewItem, EItemType, Type, int32, SlotIndex, int32, ItemIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnActiveItemChanged, FStoredItem, OldItem, FStoredItem, NewItem, EItemType, Type, int32, SlotIndex, int32, ActiveIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSlotHiddenChanged, EItemType, SlotType, int32, SlotIndex, FStoredItem, ActiveItem, bool, bIsHidden);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXEquipmentManagerComponent();

	UPROPERTY(BlueprintAssignable)
		FOnInCombatChanged OnInCombatChanged;

	UPROPERTY(BlueprintAssignable)
		FOnItemInSlotChanged OnItemInSlotChanged;

	UPROPERTY(BlueprintAssignable)
		FOnActiveItemChanged OnActiveItemChanged;

	UPROPERTY(BlueprintAssignable)
		FOnSlotHiddenChanged OnSlotHiddenChanged;

	UPROPERTY(BlueprintAssignable)
		FOnMainHandTypeChanged OnMainHandTypeChanged;

	UPROPERTY(BlueprintAssignable)
		FOnCombatTypeChanged OnCombatTypeChanged;

	UPROPERTY(BlueprintAssignable)
		FOnWeaponTypeChanged OnWeaponTypeChanged;

	UFUNCTION(BlueprintCallable, Category = XYX)
		EItemType GetItemType(FStoredItem Item);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsItemValid(FStoredItem Item);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsItemIndexValid(EItemType Type, int32 SlotIndex, int32 ItemIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SetItemInSlot(EItemType Type, int32 SlotIndex, int32 ItemIndex, FStoredItem Item);

	UFUNCTION(BlueprintCallable, Category = XYX)
		int32 GetActiveItemIndex(EItemType Type, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsSlotIndexValid(EItemType Type, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void AttachDisplayedItem(EItemType Type, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsItemTwoHanded(FStoredItem Item);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void UpdateDisplayedItem(EItemType Type, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		AXYXDisplayedItem* GetDisplayedItem(EItemType Type, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsItemEquipped(FGuid ItemId);

	UFUNCTION(BlueprintCallable, Category = XYX)
		FStoredItem GetActiveItem(EItemType Type, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsSlotHidden(EItemType Type, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SetSlotHidden(EItemType Type, int32 SlotIndex, bool bIsHidden);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void UpdateItemInSlot(EItemType Type, int32 SlotIndex, int32 ItemIndex, FStoredItem Item, EHandleSameItemMethod HandleSameItemMethod);

	UFUNCTION(BlueprintCallable, Category = XYX)
		int32 GetEquipmentSlotsIndex(EItemType Type);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void ActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		int32 GetNextArrayIndex(TArray<FStoredItem>& Wildcard, int32 Index, bool bForward);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SwitchSlotActiveIndex(EItemType Type, int32 SlotIndex, bool bForward, bool bIgnoreEmptyItems);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SwitchMainHandType(bool bForward);

	UFUNCTION(BlueprintCallable, Category = XYX)
		FStoredItem GetWeapon();

	UFUNCTION(BlueprintCallable, Category = XYX)
		EItemType GetSelectedMainHandType();

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsItemActive(FGuid ItemId);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsActiveItemIndex(EItemType Type, int32 SlotIndex, int32 ItemIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void UseActiveItemAtSlot(EItemType Type, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void FindItem(FStoredItem Item, EItemType& Type, int32& SlotIndex, int32& ItemIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void BuildEquipment(TArray<FEquipmentSlots>& Equipment);

	UFUNCTION(BlueprintCallable, Category = XYX)
		FStoredItem	GetItemInSlot(EItemType Type, int32 SlotIndex, int32 ItemIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SetSlotActiveIndex(EItemType Type, int32 SlotIndex, int32 NewActiveIndex);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SetMainHandType(EItemType Type);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsShieldEquipped();

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool GetIsInCombat();

	UFUNCTION(BlueprintCallable, Category = XYX)
		void ToggleCombat();

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SetCombat(bool bValue);

	UFUNCTION(BlueprintCallable, Category = XYX)
		ECombatType GetCombatType();

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool CanBlock();

	UFUNCTION(BlueprintCallable, Category = XYX)
		void UpdateCombatType();

	UFUNCTION(BlueprintCallable, Category = XYX)
		void GetBlockValue(float& Value, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool AreArrowEquipped();

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsTwoHandedWeaponEquipped();

	UFUNCTION(BlueprintCallable, Category = XYX)
		EWeaponType GetWeaponType();

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool IsWeaponEquipped();

	UFUNCTION(BlueprintCallable, Category = XYX)
		void Initialize();

	UFUNCTION(BlueprintCallable, Category = XYX)
		void HandleOnItemModified(FStoredItem Item);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void HandleOnGameLoaded();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXInventoryManagerComponent* InventoryComp;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TMap<EItemType, FDisplayedItems> MapDisplayedItems = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		TArray<FEquipmentSlots> EquipmentSlots = {};

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<EItemType> MainHandTypes = {
			EItemType::EMeleeWeaponRight,
			EItemType::ERangeWeapon,
			EItemType::ESpell
		};

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EItemType SelectMainHandType = EItemType::EMeleeWeaponRight;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsInCombat = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		ECombatType CombatType = ECombatType::EUnarmed;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EWeaponType WeaponType = EWeaponType::ENone;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FGuid> EquippedItems = {};

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FGuid> ActiveItems = {};


};
