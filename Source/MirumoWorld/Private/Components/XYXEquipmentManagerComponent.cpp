// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXEquipmentManagerComponent.h"
#include "Items/DisplayedItems/XYXDisplayedItem.h"
#include "Components/XYXInventoryManagerComponent.h"
#include "Items/ObjectItems/XYXItemBase.h"
#include "Actors/XYXCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/ObjectItems/XYXItemWeapon.h"

// Sets default values for this component's properties
UXYXEquipmentManagerComponent::UXYXEquipmentManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UXYXEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<AXYXCharacter>(GetOwner());


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
		UWorld* World = GetWorld();
		check(World);
		UXYXItemBase* ItemBase = Cast<UXYXItemBase>(World->SpawnActor(Item.ItemClass));
		if (ItemBase)
		{
			return ItemBase->bIsTwoHand;
		}
	}

	return false;
}

bool UXYXEquipmentManagerComponent::IsItemNoShield(FStoredItem Item)
{
	if (UKismetSystemLibrary::IsValidClass(Item.ItemClass))
	{
		UWorld* World = GetWorld();
		check(World);
		UXYXItemBase* ItemBase = Cast<UXYXItemBase>(World->SpawnActor(Item.ItemClass));
		if (ItemBase)
		{
			return ItemBase->bNoShield;
		}
	}

	return true;
}

void UXYXEquipmentManagerComponent::UpdateDisplayedItem(EItemType Type, int32 SlotIndex)
{

}

AXYXDisplayedItem* UXYXEquipmentManagerComponent::GetDisplayedItem(EItemType Type, int32 SlotIndex)
{
	return nullptr;
}

bool UXYXEquipmentManagerComponent::IsItemEquipped(FGuid ItemId)
{
	return false;
}

FStoredItem UXYXEquipmentManagerComponent::GetActiveItem(EItemType Type, int32 SlotIndex)
{
	 FStoredItem Item;
	 return Item;
}

bool UXYXEquipmentManagerComponent::IsSlotHidden(EItemType Type, int32 SlotIndex)
{
	return false;
}

void UXYXEquipmentManagerComponent::SetSlotHidden(EItemType Type, int32 SlotIndex, bool bIsHidden)
{

}

void UXYXEquipmentManagerComponent::UpdateItemInSlot(EItemType Type, int32 SlotIndex, int32 ItemIndex, FStoredItem Item, EHandleSameItemMethod HandleSameItemMethod)
{

}

int32 UXYXEquipmentManagerComponent::GetEquipmentSlotsIndex(EItemType Type)
{
	return 0;
}

void UXYXEquipmentManagerComponent::ActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex)
{

}

void UXYXEquipmentManagerComponent::SwitchSlotActiveIndex(EItemType Type, int32 SlotIndex, bool bForward, bool bIgnoreEmptyItems)
{

}

void UXYXEquipmentManagerComponent::SwitchMainHandType(bool bForward)
{

}

FStoredItem UXYXEquipmentManagerComponent::GetWeapon()
{
	FStoredItem Item;
	return Item;
}

EItemType UXYXEquipmentManagerComponent::GetSelectedMainHandType()
{
	return EItemType::ENone;
}

bool UXYXEquipmentManagerComponent::IsItemActive(FGuid ItemId)
{
	return false;
}

bool UXYXEquipmentManagerComponent::IsActiveItemIndex(EItemType Type, int32 SlotIndex, int32 ItemIndex)
{
	return false;
}

void UXYXEquipmentManagerComponent::UseActiveItemAtSlot(EItemType Type, int32 SlotIndex)
{

}

void UXYXEquipmentManagerComponent::FindItem(FStoredItem Item, EItemType& Type, int32& SlotIndex, int32& ItemIndex)
{

}

void UXYXEquipmentManagerComponent::BuildEquipment(TArray<FEquipmentSlots>& Equipment)
{
	TArray<FEquipmentSlots> EquipmentCopy;
	for (auto& e : Equipment)
	{
		EquipmentCopy.Emplace(e);
	}

	// Clear and rebuild displayed items array
	for (auto& e : DisplayedItems)
	{
		for (auto& o : e.Value.DisplayedItems)
		{
			if (IsValid(o))
			{
				GetWorld()->DestroyActor(o);
			}
		}
	}
	DisplayedItems.Empty();

	for (auto& e : EquipmentCopy)
	{
		int32 Num = e.Slots.Num();
		TArray<AXYXDisplayedItem*>  TempItems;
		TempItems.SetNum(Num);
		FDisplayedItems TempDisplayedItems;
		TempDisplayedItems.DisplayedItems = TempItems;
		DisplayedItems.Add(e.Type, TempDisplayedItems);
	}

	// Unequip all items
	for (int i = 0; i<EquipmentSlots.Num(); ++i)
	{
		EItemType Type = EquipmentSlots[i].Type;
		for (int j = 0; j<EquipmentSlots[i].Slots.Num(); ++j)
		{
			int32 SlotIndex = j;
			for (int k = 0; k < EquipmentSlots[i].Slots[j].Items.Num(); ++k)
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
	for (int i = 0; i<MainHandTypes.Num(); ++i)
	{
		int32 EqSlotsIndex = GetEquipmentSlotsIndex(MainHandTypes[i]);
		if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
		{
			for (int j = 0; j < EquipmentSlots[EqSlotsIndex].Slots.Num(); ++j)
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
	for (int i = 0; i < EquipmentCopy.Num(); ++i)
	{
		EItemType Type = EquipmentCopy[i].Type;
		for (int j = 0; j < EquipmentCopy[i].Slots.Num(); ++j)
		{
			int32 SlotIndex = j;
			SetSlotActiveIndex(Type, SlotIndex, EquipmentCopy[i].Slots[j].ActiveItemIndex);
			for (int k = 0; k < EquipmentCopy[i].Slots[j].Items.Num(); ++k)
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
		if (IsItemNoShield(Item))
		{
			SetSlotHidden(EItemType::EShield, 0, true);
		}
		else
		{
			SetSlotHidden(EItemType::EShield, 0, false);
		}
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
		UWorld* World = GetWorld();
		check(World);
		UXYXItemBase* ConstructedItem = Cast<UXYXItemBase>(World->SpawnActor(Item.ItemClass));
		if (ConstructedItem)
		{
			return ConstructedItem->BlockValue > 0;
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
	bIsInCombat = !bIsInCombat;
	SetCombat(bIsInCombat);
}

void UXYXEquipmentManagerComponent::SetCombat(bool bValue)
{
	if (bIsInCombat != bValue)
	{
		bIsInCombat = bValue;
		AttachDisplayedItem(SelectMainHandType, 0);
		AttachDisplayedItem(EItemType::EShield, 0);
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
		case EItemType::EMeleeWeapon:
			CombatType = ECombatType::EMelee;
			break;
		case EItemType::ERangeWeapon:
			CombatType = ECombatType::ERanged;
			break;
		}

		UXYXItemWeapon* Item = Cast<UXYXItemWeapon>(Weapon.ItemClass);
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
			UWorld* World = GetWorld();
			check(World);
			UXYXItemBase* ConstructedItem = Cast<UXYXItemBase>(World->SpawnActor(Item.ItemClass));
			if (ConstructedItem && ConstructedItem->BlockValue > 0)
			{
				Value = ConstructedItem->BlockValue;
				bSuccess = true;
			}
		}
	}

	ItemIndex = GetActiveItemIndex(SelectMainHandType, 0);
	Item = GetItemInSlot(SelectMainHandType, 0, ItemIndex);
	if (IsItemValid(Item))
	{
		if (!IsSlotHidden(SelectMainHandType, 0))
		{
			UWorld* World = GetWorld();
			check(World);
			UXYXItemBase* ConstructedItem = Cast<UXYXItemBase>(World->SpawnActor(Item.ItemClass));
			if (ConstructedItem && ConstructedItem->BlockValue > 0)
			{
				Value = ConstructedItem->BlockValue;
				bSuccess = true;
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


