// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXEquipmentManagerComponent.h"
#include "Items/DisplayedItems/XYXDisplayedItem.h"
#include "Components/XYXInventoryManagerComponent.h"
#include "Items/ObjectItems/XYXItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/ObjectItems/XYXItemWeapon.h"
#include "Game/XYXData.h"
#include "Interfaces/XYXInterfaceItem.h"
#include <Game/XYXGameMode.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values for this component's properties
UXYXEquipmentManagerComponent::UXYXEquipmentManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UXYXEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FDisplayedItems> DIValues;
	MapDisplayedItems.GenerateValueArray(DIValues);
	for (int32 i = 0; i < DIValues.Num(); ++i)
	{
		for (int32 j = 0; j < DIValues[i].DisplayedItems.Num(); ++j)
		{
			auto& o = DIValues[i].DisplayedItems[j];
			if (IsValid(o))
			{
				GetWorld()->DestroyActor(o);
			}
		}
	}

	MapDisplayedItems.Empty();
}

EItemType UXYXEquipmentManagerComponent::GetItemType(FStoredItem Item)
{
	TSubclassOf<class UXYXItemBase> ItemClass = Item.ItemClass;
	const UXYXItemBase* const ItemBase = ItemClass.GetDefaultObject();
	if (ItemBase)
	{
		return ItemBase->Item.Type;
	}

	return  EItemType::ENone;
}

bool UXYXEquipmentManagerComponent::IsItemValid(FStoredItem Item)
{
	return UKismetSystemLibrary::IsValidClass(Item.ItemClass) && Item.Amount > 0;
}

bool UXYXEquipmentManagerComponent::IsItemIndexValid(EItemType Type, int32 SlotIndex, int32 ItemIndex)
{
	int32 Index = GetEquipmentSlotsIndex(Type);
	if (EquipmentSlots.IsValidIndex(Index))
	{
		if (EquipmentSlots[Index].Slots.IsValidIndex(SlotIndex))
		{
			if (EquipmentSlots[Index].Slots[SlotIndex].Items.IsValidIndex(ItemIndex))
			{
				return true;
			}
		}
	}

	return false;
}

void UXYXEquipmentManagerComponent::SetItemInSlot(EItemType Type, int32 SlotIndex, int32 ItemIndex, FStoredItem Item)
{
	int32 Index = GetEquipmentSlotsIndex(Type);
	if (EquipmentSlots.IsValidIndex(Index))
	{
		if (EquipmentSlots[Index].Slots.IsValidIndex(SlotIndex))
		{
			EquipmentSlots[Index].Slots[SlotIndex].Items[ItemIndex] = Item;
		}
	}
}

int32 UXYXEquipmentManagerComponent::GetActiveItemIndex(EItemType Type, int32 SlotIndex)
{
	int32 Index = GetEquipmentSlotsIndex(Type);
	if (EquipmentSlots.IsValidIndex(Index))
	{
		if (EquipmentSlots[Index].Slots.IsValidIndex(SlotIndex))
		{
			return EquipmentSlots[Index].Slots[SlotIndex].ActiveItemIndex;
		}
	}

	return 0;
}

bool UXYXEquipmentManagerComponent::IsSlotIndexValid(EItemType Type, int32 SlotIndex)
{
	int32 Index = GetEquipmentSlotsIndex(Type);
	if (EquipmentSlots.IsValidIndex(Index))
	{
		if (EquipmentSlots[Index].Slots.IsValidIndex(SlotIndex))
		{
			return true;
		}
	}

	return false;
}

void UXYXEquipmentManagerComponent::AttachDisplayedItem(EItemType Type, int32 SlotIndex)
{
	AXYXDisplayedItem* DisplayedItem = GetDisplayedItem(Type, SlotIndex);
	if (IsValid(DisplayedItem))
	{
		DisplayedItem->Attach();
	}
}

bool UXYXEquipmentManagerComponent::IsItemTwoHanded(FStoredItem Item)
{
	if (UKismetSystemLibrary::IsValidClass(Item.ItemClass))
	{
		UXYXItemBase* ItemBase = NewObject<UXYXItemBase>(this, Item.ItemClass);
		if (ItemBase)
		{
			IXYXInterfaceItem* MyInterface = Cast<IXYXInterfaceItem>(ItemBase);
			if (MyInterface)
			{
				return MyInterface->Execute_IsTwoHanded(ItemBase);
			}
		}
	}

	return false;
}

void UXYXEquipmentManagerComponent::UpdateDisplayedItem(EItemType Type, int32 SlotIndex)
{
	TArray<EItemType>   DIKeys;
	TArray<FDisplayedItems> DIValues;
	MapDisplayedItems.GenerateKeyArray(DIKeys); //生成key、value数组
	MapDisplayedItems.GenerateValueArray(DIValues);

	int32 FoundIndex;
	if (DIKeys.Find(Type, FoundIndex))
	{
		if (FoundIndex < 0)
		{
			return;
		}
	}

	// Destroy displayed item in slot index
	if (DIValues[FoundIndex].DisplayedItems.IsValidIndex(SlotIndex))
	{
		auto& o = DIValues[FoundIndex].DisplayedItems[SlotIndex];
		if (IsValid(o))
		{
			GetWorld()->DestroyActor(o);
		}
	}

	// Spawn displayed item if possible
	FStoredItem Item = GetActiveItem(Type, SlotIndex);
	if (UKismetSystemLibrary::IsValidClass(Item.ItemClass))
	{
		UXYXItemBase* ItemBase = NewObject<UXYXItemBase>(this, Item.ItemClass);
		if (ItemBase)
		{
			IXYXInterfaceItem* MyInterface = Cast<IXYXInterfaceItem>(ItemBase);
			if (MyInterface)
			{
				TArray<AXYXDisplayedItem*>  TempArray;
				TempArray = DIValues[FoundIndex].DisplayedItems;

				auto DIClass = MyInterface->Execute_GetDisplayedItem(ItemBase);
				if (UKismetSystemLibrary::IsValidClass(DIClass))
				{
					UWorld* const World = GetWorld();
					check(World);

					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					ActorSpawnParams.Owner = GetOwner();
					ActorSpawnParams.Instigator = GetOwner()->GetInstigator();
					AXYXDisplayedItem* XYXActor = World->SpawnActor<AXYXDisplayedItem>(DIClass, FTransform(), ActorSpawnParams);
					if (XYXActor)
					{
						XYXActor->SetEquipmemtComp(this);
						XYXActor->SetItemType(Type);
						XYXActor->SetSlotIndex(SlotIndex);

						TempArray[SlotIndex] = XYXActor;
						FDisplayedItems NewDisplayedItems;
						NewDisplayedItems.DisplayedItems = TempArray;

						MapDisplayedItems.Add(Type, NewDisplayedItems);
					}
				}
			}
		}
	}
}

AXYXDisplayedItem* UXYXEquipmentManagerComponent::GetDisplayedItem(EItemType Type, int32 SlotIndex)
{
	AXYXDisplayedItem* DisplayItem = nullptr;
	if (MapDisplayedItems.Contains(Type))
	{
		auto& FItems = MapDisplayedItems[Type];
		if (FItems.DisplayedItems.IsValidIndex(SlotIndex))
		{
			DisplayItem = FItems.DisplayedItems[SlotIndex];
		}
	}

	return DisplayItem;
}

bool UXYXEquipmentManagerComponent::IsItemEquipped(FGuid ItemId)
{
	int32 Index;
	if (EquippedItems.Find(ItemId, Index))
	{
		if (Index >= 0)
		{
			return true;
		}
	}

	return false;
}

FStoredItem UXYXEquipmentManagerComponent::GetActiveItem(EItemType Type, int32 SlotIndex)
{
	FStoredItem Item;
	if (IsSlotIndexValid(Type, SlotIndex))
	{
		int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);
		if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
		{
			if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(SlotIndex))
			{
				auto& Slots = EquipmentSlots[EqSlotsIndex].Slots[SlotIndex];
				int32 ActiveItemIndex = Slots.ActiveItemIndex;
				if (Slots.Items.IsValidIndex(ActiveItemIndex))
				{
					Item = Slots.Items[ActiveItemIndex];
				}
			}
		}
	}

	return Item;
}

bool UXYXEquipmentManagerComponent::IsSlotHidden(EItemType Type, int32 SlotIndex)
{
	if (IsSlotIndexValid(Type, SlotIndex))
	{
		int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);
		if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
		{
			if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(SlotIndex))
			{
				return EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].bIsHidden;
			}
		}
	}

	return false;
}

void UXYXEquipmentManagerComponent::SetSlotHidden(EItemType Type, int32 SlotIndex, bool bIsHidden)
{
	if (IsSlotHidden(Type, SlotIndex) != bIsHidden)
	{
		int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);
		if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
		{
			if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(SlotIndex))
			{
				EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].bIsHidden = bIsHidden;
				int32 ActiveIndex = GetActiveItemIndex(Type, SlotIndex);
				FStoredItem ActiveItem = GetItemInSlot(Type, SlotIndex, ActiveIndex);
				OnSlotHiddenChanged.Broadcast(Type, SlotIndex, ActiveItem, bIsHidden);
			}
		}
	}
}

void UXYXEquipmentManagerComponent::UpdateItemInSlot(EItemType Type, int32 SlotIndex, int32 ItemIndex, FStoredItem Item, EHandleSameItemMethod HandleSameItemMethod)
{
	if(!IsItemIndexValid(Type, SlotIndex, ItemIndex))
	{
		return;
	}

	if (IsItemValid(Item))
	{
		if (Type == GetItemType(Item))
		{
			FStoredItem OldItem = GetItemInSlot(Type, SlotIndex, ItemIndex);
			bool bEquipNewItem = false;
			if (Item.Id == OldItem.Id)
			{
				// if item is the same, unequip it
				if(HandleSameItemMethod == EHandleSameItemMethod::EUnequip)
				{
					FStoredItem EmptyItem;
					SetItemInSlot(Type, SlotIndex, ItemIndex, EmptyItem);
					EquippedItems.Remove(OldItem.Id);
					OnItemInSlotChanged.Broadcast(OldItem, EmptyItem, Type, SlotIndex, ItemIndex);
					if (IsActiveItemIndex(Type, SlotIndex, ItemIndex))
					{
						ActiveItemChanged(OldItem, EmptyItem, Type, SlotIndex, ItemIndex);
					}
				}
				else if(HandleSameItemMethod == EHandleSameItemMethod::EUpdate)
				{
					bEquipNewItem = true;
				}
			}
			else
			{
				bEquipNewItem = true;
				// check if that item was already equipped in other slot index, if so, unequip it
				int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);
				if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
				{
					auto& Slots = EquipmentSlots[EqSlotsIndex].Slots;
					for (int32 i = 0; i < Slots.Num(); ++i)
					{
						for (int32 j = 0; j < Slots[i].Items.Num(); ++j)
						{
							if (Slots[i].Items[j].Id == Item.Id)
							{
								OldItem = Slots[i].Items[j];
								FStoredItem EmptyItem;
								SetItemInSlot(Type, i, j, EmptyItem);
								EquippedItems.Remove(OldItem.Id);
								OnItemInSlotChanged.Broadcast(OldItem, EmptyItem, Type, i, j);
								if (IsActiveItemIndex(Type, i, j))
								{
									ActiveItemChanged(OldItem, EmptyItem, Type, i, j);
									break;
								}
							}
						}
					}
				}
			}

			if (bEquipNewItem)
			{
				OldItem = GetItemInSlot(Type, SlotIndex, ItemIndex);
				SetItemInSlot(Type, SlotIndex, ItemIndex, Item);
				EquippedItems.Remove(OldItem.Id);
				EquippedItems.Add(Item.Id);
				OnItemInSlotChanged.Broadcast(OldItem, Item, Type, SlotIndex, ItemIndex);
				if (IsActiveItemIndex(Type, SlotIndex, ItemIndex))
				{
					ActiveItemChanged(OldItem, Item, Type, SlotIndex, ItemIndex);
				}
			}
		}
	}
	else
	{
		FStoredItem OldItem = GetItemInSlot(Type, SlotIndex, ItemIndex);
		if (IsItemValid(OldItem))
		{
			FStoredItem EmptyItem;
			SetItemInSlot(Type, SlotIndex, ItemIndex, EmptyItem);
			EquippedItems.Remove(OldItem.Id);
			OnItemInSlotChanged.Broadcast(OldItem, EmptyItem, Type, SlotIndex, ItemIndex);
			if (IsActiveItemIndex(Type, SlotIndex, ItemIndex))
			{
				ActiveItemChanged(OldItem, EmptyItem, Type, SlotIndex, ItemIndex);
			}
		}
	}
}

int32 UXYXEquipmentManagerComponent::GetEquipmentSlotsIndex(EItemType Type)
{
	int32 EqSlotsIndex = -1;
	for (int32 i = 0; i < EquipmentSlots.Num(); ++i)
	{
		if (Type == EquipmentSlots[i].Type)
		{
			EqSlotsIndex = i;
			break;
		}
	}

	return EqSlotsIndex;
}

void UXYXEquipmentManagerComponent::ActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex)
{
	if (Type == SelectMainHandType)
	{
		UpdateCombatType();
		if (IsItemTwoHanded(NewItem))
		{
			SetSlotHidden(EItemType::EShield, 0, true);
		}
		else
		{
			SetSlotHidden(EItemType::EShield, 0, false);
		}
		bool bShowArrows = SelectMainHandType == EItemType::ERangeWeapon && GetCombatType() == ECombatType::ERanged;
		SetSlotHidden(EItemType::EArrows, 0, !bShowArrows);
	}

	if (OldItem.Id != NewItem.Id)
	{
		ActiveItems.Remove(OldItem.Id);
		ActiveItems.Add(NewItem.Id);
		UpdateDisplayedItem(Type, SlotIndex);
		AttachDisplayedItem(Type, SlotIndex);
	}
		
	OnActiveItemChanged.Broadcast(OldItem, NewItem, Type, SlotIndex, ActiveIndex);
}

int32 UXYXEquipmentManagerComponent::GetNextArrayIndex(TArray <FStoredItem> &Wildcard, int32 Index, bool bForward)
{
	int32 NewIndex = 0;
	if (bForward)
	{
		if (Wildcard.IsValidIndex(Index + 1))
		{
			NewIndex = Index + 1;
		}
		else
		{
			NewIndex = 0;
		}
	}
	else
	{
		if (Wildcard.IsValidIndex(Index - 1))
		{
			NewIndex = Index - 1;
		}
		else
		{
			NewIndex = Wildcard.Num() - 1;
		}
	}

	return NewIndex;
}

void UXYXEquipmentManagerComponent::SwitchSlotActiveIndex(EItemType Type, int32 SlotIndex, bool bForward, bool bIgnoreEmptyItems)
{
	if (!IsSlotIndexValid(Type,SlotIndex))
	{
		return;
	}

	int32 ActiveIndex = GetActiveItemIndex(Type, SlotIndex);
	int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);
	if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
	{
		if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(SlotIndex))
		{
			bool ChangeActiveItem = false;
			auto& Slots = EquipmentSlots[EqSlotsIndex].Slots[SlotIndex];
			int32 NewIndex = GetNextArrayIndex(Slots.Items, ActiveIndex, bForward);
			if (bIgnoreEmptyItems)
			{
				for (int32 i = 1; i < Slots.Items.Num(); ++i)
				{
					FStoredItem Item = Slots.Items[NewIndex];
					if (IsItemValid(Item))
					{
						ChangeActiveItem = true;
					}
					else
					{
						NewIndex = GetNextArrayIndex(Slots.Items, NewIndex, bForward);
					}
				}
			}
			else
			{
				ChangeActiveItem = true;
			}

			if (ChangeActiveItem)
			{
				Slots.ActiveItemIndex = NewIndex;
				if (Slots.Items.IsValidIndex(ActiveIndex) && Slots.Items.IsValidIndex(NewIndex))
				{
					FStoredItem OldItem = Slots.Items[ActiveIndex];
					FStoredItem NewItem = Slots.Items[NewIndex];
					ActiveItemChanged(OldItem, NewItem, Type, SlotIndex, NewIndex);
				}
			}
		}
	}
}

void UXYXEquipmentManagerComponent::SwitchMainHandType(bool bForward)
{
	int32 SelectedIndex;
	if (MainHandTypes.Find(SelectMainHandType, SelectedIndex))
	{
		if (SelectedIndex < 0)
		{
			return;
		}
	}

	int32 NextIndex = 0;
	if (bForward)
	{
		if (MainHandTypes.IsValidIndex(SelectedIndex + 1))
		{
			NextIndex = SelectedIndex + 1;
		}
		else
		{
			NextIndex = 0;
		}
	}
	else
	{
		if (MainHandTypes.IsValidIndex(SelectedIndex - 1))
		{
			NextIndex = SelectedIndex - 1;
		}
		else
		{
			NextIndex = MainHandTypes.Num() - 1;
		}
	}

	if (MainHandTypes.IsValidIndex(NextIndex))
	{
		SetMainHandType(MainHandTypes[NextIndex]);
	}
}

FStoredItem UXYXEquipmentManagerComponent::GetWeapon()
{
	int32 ItemIndex = GetActiveItemIndex(SelectMainHandType, 0);
	return GetItemInSlot(SelectMainHandType, 0, ItemIndex);
}

EItemType UXYXEquipmentManagerComponent::GetSelectedMainHandType()
{
	return SelectMainHandType;
}

bool UXYXEquipmentManagerComponent::IsItemActive(FGuid ItemId)
{
	int32 Index;
	if (ActiveItems.Find(ItemId, Index))
	{
		return Index >= 0;
	}

	return false;
}

bool UXYXEquipmentManagerComponent::IsActiveItemIndex(EItemType Type, int32 SlotIndex, int32 ItemIndex)
{
	int32 ActiveIndex = GetActiveItemIndex(Type, SlotIndex);
	return ActiveIndex == ItemIndex;
}

void UXYXEquipmentManagerComponent::UseActiveItemAtSlot(EItemType Type, int32 SlotIndex)
{
	int32 ItemIndex = GetActiveItemIndex(Type, SlotIndex);
	if (IsItemIndexValid(Type, SlotIndex, ItemIndex))
	{
		int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);
		if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
		{
			if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(SlotIndex))
			{
				auto& Slots = EquipmentSlots[EqSlotsIndex].Slots[SlotIndex];
				if (Slots.Items.IsValidIndex(ItemIndex) && !Slots.bIsHidden)
				{
					FStoredItem Item = Slots.Items[ItemIndex];
					if (IsItemValid(Item))
					{
						if (IsValid(InventoryComp))
						{
							InventoryComp->UseItem(Item.Id);
						}
						else
						{
							UXYXItemBase* ItemBase = NewObject<UXYXItemBase>(this, Item.ItemClass);
							if (ItemBase)
							{
								ItemBase->UseItem(GetOwner());
							}
							Item.Amount -= 1;
							UpdateItemInSlot(Type, SlotIndex, ItemIndex, Item, EHandleSameItemMethod::EUpdate);
						}
					}
				}
			}
		}
	}
}

void UXYXEquipmentManagerComponent::FindItem(FStoredItem Item, EItemType& Type, int32& SlotIndex, int32& ItemIndex) 
{
	Type = EItemType::ENone;
	SlotIndex = -1;
	ItemIndex = -1;

	EItemType ItemType = GetItemType(Item);
	int32 EqSlotsIndex = GetEquipmentSlotsIndex(ItemType);
	if (EqSlotsIndex >= 0)
	{
		if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
		{
			for (int32 i = 0; i < EquipmentSlots[EqSlotsIndex].Slots.Num(); ++i)
			{
				for (int32 j = 0; j < EquipmentSlots[EqSlotsIndex].Slots[i].Items.Num(); ++j)
				{
					if (EquipmentSlots[EqSlotsIndex].Slots[i].Items[j].Id == Item.Id)
					{
						Type = EquipmentSlots[EqSlotsIndex].Type;
						SlotIndex = i;
						ItemIndex = j;
						break;
					}
				}
			}
		}
	}
}

void UXYXEquipmentManagerComponent::BuildEquipment(TArray<FEquipmentSlots>& Equipment)
{
	TArray<FEquipmentSlots> EquipmentCopy;
	for (auto& e : Equipment)
	{
		EquipmentCopy.Emplace(e);
	}

	// Clear and rebuild displayed items array
	for (auto& e : MapDisplayedItems)
	{
		for (auto& o : e.Value.DisplayedItems)
		{
			if (IsValid(o))
			{
				GetWorld()->DestroyActor(o);
			}
		}
	}

	MapDisplayedItems.Empty();

	for (auto& e : EquipmentCopy)
	{
		int32 Num = e.Slots.Num();
		TArray<AXYXDisplayedItem*>  TempItems;
		TempItems.SetNum(Num);
		FDisplayedItems TempDisplayedItems;
		TempDisplayedItems.DisplayedItems = TempItems;
		MapDisplayedItems.Add(e.Type, TempDisplayedItems);
	}

	// Unequip all items
	for (int32 i = 0; i<EquipmentSlots.Num(); ++i)
	{
		EItemType Type = EquipmentSlots[i].Type;
		for (int32 j = 0; j<EquipmentSlots[i].Slots.Num(); ++j)
		{
			int32 SlotIndex = j;
			for (int32 k = 0; k < EquipmentSlots[i].Slots[j].Items.Num(); ++k)
			{
				int32 ItemIndex = k;
				FStoredItem Item = EquipmentSlots[i].Slots[j].Items[k];
				if (IsItemValid(Item) && IsItemEquipped(Item.Id))
				{
					FStoredItem EmptyItem;
					UpdateItemInSlot(Type, SlotIndex, ItemIndex, EmptyItem, EHandleSameItemMethod::EUnequip);
				}
				else
				{
					FStoredItem EmptyItem;
					SetItemInSlot(Type, SlotIndex, ItemIndex, EmptyItem);
				}
			}
		}
	}

	// Hide not selected main hand slot types 
	// (hide all and show only active to make sure OnSlotHiddenChanged event dispatcher will be triggered)
	for (int32 i = 0; i<MainHandTypes.Num(); ++i)
	{
		int32 EqSlotsIndex = GetEquipmentSlotsIndex(MainHandTypes[i]);
		if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
		{
			for (int32 j = 0; j < EquipmentSlots[EqSlotsIndex].Slots.Num(); ++j)
			{
				EItemType Type = EquipmentSlots[EqSlotsIndex].Type;
				if (Type == SelectMainHandType)
				{
					SetSlotHidden(Type, j, false);
				}
				else
				{
					SetSlotHidden(Type, j, true);
				}
			}
		}
	}

	// Equip new items
	for (int32 i = 0; i < EquipmentCopy.Num(); ++i)
	{
		EItemType Type = EquipmentCopy[i].Type;
		for (int32 j = 0; j < EquipmentCopy[i].Slots.Num(); ++j)
		{
			int32 SlotIndex = j;
			SetSlotActiveIndex(Type, SlotIndex, EquipmentCopy[i].Slots[j].ActiveItemIndex);
			for (int32 k = 0; k < EquipmentCopy[i].Slots[j].Items.Num(); ++k)
			{
				int32 ItemIndex = k;
				FStoredItem Item = EquipmentCopy[i].Slots[j].Items[k];
				if (IsValid(InventoryComp))
				{
					if (UKismetSystemLibrary::IsValidClass(Item.ItemClass))
					{
						int32 FoundItemIndex;
						if (Item.Id.IsValid())
							FoundItemIndex = InventoryComp->FindIndexById(Item.Id);
						else
							FoundItemIndex = InventoryComp->FindIndexByClass(Item.ItemClass);

						if (FoundItemIndex >= 0)
						{
							FStoredItem FoundItem = InventoryComp->GetItemAtIndex(FoundItemIndex);
							UpdateItemInSlot(Type, SlotIndex, ItemIndex, FoundItem, EHandleSameItemMethod::EUpdate);
						}
					}
				}
				else
				{
					if (IsItemValid(Item))
					{
						UpdateItemInSlot(Type, SlotIndex, ItemIndex, Item, EHandleSameItemMethod::EUpdate);
					}
				}
			}
		}
	}

	// Update Combat Type
	UpdateCombatType();
}

FStoredItem UXYXEquipmentManagerComponent::GetItemInSlot(EItemType Type, int32 SlotIndex, int32 ItemIndex)
{
	FStoredItem Item = {};
	if (IsSlotIndexValid(Type, SlotIndex))
	{
		int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);
		if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
		{
			if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(SlotIndex))
			{
				if (EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].Items.IsValidIndex(ItemIndex))
				{
					Item = EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].Items[ItemIndex];
				}
			}
		}
	}

	return Item;
}

void UXYXEquipmentManagerComponent::SetSlotActiveIndex(EItemType Type, int32 SlotIndex, int32 NewActiveIndex)
{
	if (!IsSlotIndexValid(Type, SlotIndex))
	{
		return;
	}

	int32 ActiveIndex = GetActiveItemIndex(Type, SlotIndex);
	int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);

	if (ActiveIndex != NewActiveIndex && ActiveIndex >= 0 && NewActiveIndex >= 0)
	{
		if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
		{
			if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(SlotIndex))
			{
				if (EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].Items.IsValidIndex(NewActiveIndex))
				{
					EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].ActiveItemIndex = NewActiveIndex;
					FStoredItem OldItem = EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].Items[ActiveIndex];
					FStoredItem NewItem = EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].Items[NewActiveIndex];
					ActiveItemChanged(OldItem, NewItem, Type, SlotIndex, NewActiveIndex);
				}
			}
		}
	}
}

void UXYXEquipmentManagerComponent::SetMainHandType(EItemType Type)
{
	int32 Index;
	if (MainHandTypes.Find(Type,Index))
	{
		if (Index < 0)
		{
			return;
		}
	}

	if (SelectMainHandType == Type)
	{
		return;
	}

	EItemType PreviousType = SelectMainHandType;
	SelectMainHandType = Type;

	UpdateCombatType();
	SetSlotHidden(PreviousType, 0, true);
	SetSlotHidden(SelectMainHandType, 0, false);

	int32 ItemIndex = GetActiveItemIndex(SelectMainHandType, 0);
	FStoredItem Item = GetItemInSlot(SelectMainHandType, 0, ItemIndex);
	if (IsItemTwoHanded(Item))
	{
		SetSlotHidden(EItemType::EShield, 0, true);
	}
	else
	{
		SetSlotHidden(EItemType::EShield, 0, false);
	}

	bool ArrowShow = SelectMainHandType == EItemType::ERangeWeapon && GetCombatType() == ECombatType::ERanged;
	SetSlotHidden(EItemType::EArrows, 0, !ArrowShow);

	OnMainHandTypeChanged.Broadcast(SelectMainHandType);
}

bool UXYXEquipmentManagerComponent::IsShieldEquipped()
{
	if (IsSlotHidden(EItemType::EShield, 0))
	{
		return false;
	}

	int32 Index = GetActiveItemIndex(EItemType::EShield, 0);
	FStoredItem Item = GetItemInSlot(EItemType::EShield, 0, Index);
	if (IsItemValid(Item))
	{
		UXYXItemBase* ItemBase = NewObject<UXYXItemBase>(this, Item.ItemClass);
		if (ItemBase)
		{
			IXYXInterfaceItem* MyInterface = Cast<IXYXInterfaceItem>(ItemBase);
			if (MyInterface)
			{
				return MyInterface->Execute_GetBlockValue(ItemBase) > 0.f;
			}
		}
	}

	return false;
}

bool UXYXEquipmentManagerComponent::GetIsInCombat()
{
	return bIsInCombat;
}

void UXYXEquipmentManagerComponent::ToggleCombat()
{
	SetCombat(!bIsInCombat);
}

void UXYXEquipmentManagerComponent::SetCombat(bool bValue)
{
	if (bIsInCombat != bValue)
	{
		bIsInCombat = bValue;
		AttachDisplayedItem(SelectMainHandType, 0);
		if (SelectMainHandType == EItemType::EMeleeWeaponRight &&
			(WeaponType == EWeaponType::EDualSwordRight || WeaponType == EWeaponType::ETwinDaggerRight))
		{
			AttachDisplayedItem(EItemType::EMeleeWeaponLeft, 0);
		}
		else
		{
			AttachDisplayedItem(EItemType::EShield, 0);
		}

		OnInCombatChanged.Broadcast(bIsInCombat);
	}
}

ECombatType UXYXEquipmentManagerComponent::GetCombatType()
{
	return CombatType;
}

bool UXYXEquipmentManagerComponent::CanBlock()
{
	if (bIsInCombat)
	{
		return CombatType == ECombatType::EMelee || CombatType == ECombatType::EUnarmed;
	}

	return false;
}

void UXYXEquipmentManagerComponent::UpdateCombatType()
{
	ECombatType PreviousCombatType = CombatType;
	EWeaponType PreviousWeaponType = WeaponType;

	FStoredItem Weapon = GetWeapon();
	if (IsItemValid(Weapon))
	{
		switch(SelectMainHandType)
		{
		case EItemType::ESpell:
			CombatType = ECombatType::EMagic;
			break;
		case EItemType::EMeleeWeaponRight:
		case EItemType::EMeleeWeaponLeft:
			CombatType = ECombatType::EMelee;
			break;
		case EItemType::ERangeWeapon:
			CombatType = ECombatType::ERanged;
			break;
		}

		auto&& Item = NewObject<UXYXItemWeapon>(this, Weapon.ItemClass);
		if (Item)
		{
			WeaponType = Item->WeaponType;
		}
	}
	else
	{
		CombatType = ECombatType::EUnarmed;
		WeaponType = EWeaponType::ENone;
	}

	if (CombatType != PreviousCombatType)
	{
		OnCombatTypeChanged.Broadcast(CombatType);
	}

	if (WeaponType != PreviousWeaponType)
	{
		OnWeaponTypeChanged.Broadcast(WeaponType);
	}
}

void UXYXEquipmentManagerComponent::GetBlockValue(float& Value, bool& bSuccess)
{
	Value = 0.f;
	bSuccess = false;
	int32 ItemIndex = GetActiveItemIndex(EItemType::EShield, 0);
	FStoredItem Item = GetItemInSlot(EItemType::EShield, 0, ItemIndex);
	if (IsItemValid(Item))
	{
		if (!IsSlotHidden(EItemType::EShield, 0))
		{
			UXYXItemBase* ItemBase = NewObject<UXYXItemBase>(this, Item.ItemClass);
			if (ItemBase)
			{
				IXYXInterfaceItem* MyInterface = Cast<IXYXInterfaceItem>(ItemBase);
				if (MyInterface)
				{
					Value = MyInterface->Execute_GetBlockValue(ItemBase);
					bSuccess = true;
				}
			}
		}
	}

	ItemIndex = GetActiveItemIndex(SelectMainHandType, 0);
	Item = GetItemInSlot(SelectMainHandType, 0, ItemIndex);
	if (IsItemValid(Item))
	{
		if (!IsSlotHidden(SelectMainHandType, 0))
		{
			UXYXItemBase* ItemBase = NewObject<UXYXItemBase>(this, Item.ItemClass);
			if (ItemBase)
			{
				IXYXInterfaceItem* MyInterface = Cast<IXYXInterfaceItem>(ItemBase);
				if (MyInterface)
				{
					Value = MyInterface->Execute_GetBlockValue(ItemBase);
					bSuccess = true;
				}
			}
		}
	}
}

bool UXYXEquipmentManagerComponent::AreArrowEquipped()
{
	if (IsSlotHidden(EItemType::EArrows, 0))
	{
		return false;
	}
	else
	{
		int32 ItemIndex = GetActiveItemIndex(EItemType::EArrows, 0);
		FStoredItem Item = GetItemInSlot(EItemType::EArrows, 0, ItemIndex);
		return IsItemValid(Item);
	}

	return false;
}

bool UXYXEquipmentManagerComponent::IsTwoHandedWeaponEquipped()
{
	return IsItemTwoHanded(GetWeapon());
}

EWeaponType UXYXEquipmentManagerComponent::GetWeaponType()
{
	return WeaponType;
}

bool UXYXEquipmentManagerComponent::IsWeaponEquipped()
{
	return GetWeaponType() != EWeaponType::ENone;
}

void UXYXEquipmentManagerComponent::Initialize()
{
	InventoryComp = Cast<UXYXInventoryManagerComponent>(GetOwner()->GetComponentByClass(UXYXInventoryManagerComponent::StaticClass()));
	if (IsValid(InventoryComp))
	{
		InventoryComp->OnItemRemoved.AddDynamic(this, &UXYXEquipmentManagerComponent::HandleOnItemModified);
		InventoryComp->OnItemAdded.AddDynamic(this, &UXYXEquipmentManagerComponent::HandleOnItemModified);
	}

	BuildEquipment(EquipmentSlots);

	auto&& Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto&& Owner = Cast<ACharacter>(GetOwner());
	if (Owner == Character)
	{
		auto&& 	XYXGameMode = Cast<AXYXGameMode>(UGameplayStatics::GetGameMode(this));
		if (IsValid(XYXGameMode))
		{
			XYXGameMode->OnGameLoaded.AddDynamic(this, &UXYXEquipmentManagerComponent::HandleOnGameLoaded);
		}
	}

}

void UXYXEquipmentManagerComponent::HandleOnItemModified(FStoredItem Item)
{
	EItemType Type;
	int32 SlotIndex;
	int32 ItemIndex;
	FindItem(Item, Type, SlotIndex, ItemIndex);
	if (SlotIndex >= 0)
	{
		UpdateItemInSlot(Type, SlotIndex, ItemIndex, Item, EHandleSameItemMethod::EUpdate);
	}
}

void UXYXEquipmentManagerComponent::HandleOnGameLoaded()
{
	auto&& XYXGameMode = Cast<AXYXGameMode>(UGameplayStatics::GetGameMode(this));
	if (IsValid(XYXGameMode))
	{
		SelectMainHandType = XYXGameMode->SelectMainHandSlotType;
		BuildEquipment(XYXGameMode->EquipmentSlots);
		SetCombat(XYXGameMode->bIsInCombat);
	}
}

