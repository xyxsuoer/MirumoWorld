// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXInventoryManagerComponent.h"
#include <Items/ObjectItems/XYXItemBase.h>
#include "Kismet/KismetSystemLibrary.h"
#include "Actors/XYXCharacter.h"
#include "Game/XYXGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UXYXInventoryManagerComponent::UXYXInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UXYXInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	ClearInventory();

	auto&& Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto&& Owner = Cast<ACharacter>(GetOwner());
	if (Character == Owner)
	{
		auto&& XYXGameMode = Cast<AXYXGameMode>(UGameplayStatics::GetGameMode(this));
		if (IsValid(XYXGameMode))
		{
			XYXGameMode->OnGameLoaded.AddDynamic(this, &UXYXInventoryManagerComponent::HandleOnGameLoaded);
		}
	}

}

void UXYXInventoryManagerComponent::AddItem(TSubclassOf<UXYXItemBase> ItemClass, int32 Amount)
{
	if (UKismetSystemLibrary::IsValidClass(ItemClass) && Amount > 0)
	{
		const UXYXItemBase* const ItemBase = ItemClass.GetDefaultObject();
		if (ItemBase)
		{
			if (ItemBase->Item.bIsStackable)
			{
				int32 Index = FindIndexByClass(ItemClass);
				if (Index >= 0)
				{
					Inventory[Index].Amount += Amount;
					OnItemAdded.Broadcast(Inventory[Index]);
				}
				else
				{
					FStoredItem Item;
					Item.Amount = Amount;
					Item.Id = FGuid::NewGuid();
					Item.ItemClass = ItemClass;
					Inventory.Add(Item);
					OnItemAdded.Broadcast(Item);
				}
			}
			else
			{
				if (Amount > 1)
				{
					UE_LOG(LogTemp, Warning, TEXT("Warning! Tried to add more than 1 unstuckable item: %s"),*ItemClass->GetDefaultObjectName().ToString());
				}
				FStoredItem Item;
				Item.Amount = Amount;
				Item.Id = FGuid::NewGuid();
				Item.ItemClass = ItemClass;
				Inventory.Add(Item);
				OnItemAdded.Broadcast(Item);
			}
		}
	}
}

void UXYXInventoryManagerComponent::RemoveItem(TSubclassOf<class UXYXItemBase> ItemClass, int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	const UXYXItemBase* const ItemBase = ItemClass.GetDefaultObject();
	if (ItemBase->Item.bIsStackable)
	{
		int32 Index = FindIndexByClass(ItemClass);
		if (Index >= 0)
		{
			Inventory[Index].Amount -= Amount;
			ClearInventory();
		}
	}
	else
	{
		for (int i = 1; i < Amount; ++i)
		{
			int32 Index = FindIndexByClass(ItemClass);
			if (Index >= 0)
			{
				Inventory[Index] = {};
			}
		}
		ClearInventory();
	}

}

void UXYXInventoryManagerComponent::ClearInventory()
{
	int32 Index = 0;
	for (auto & e : Inventory)
	{
		if (IsSlotEmpty(Index))
		{
			Inventory.RemoveAt(Index);
		}
		else
		{
			const UXYXItemBase* const ItemBase = e.ItemClass.GetDefaultObject();
			if (!ItemBase->Item.bIsStackable)
			{
				Inventory[Index].Amount = 1;
			}
		}
		Index++;
	}
}

void UXYXInventoryManagerComponent::RemoveItemAtIndex(int32 Index, int32 Amount)
{
	if (Amount <= 0 || IsSlotEmpty(Index))
	{
		return;
	}
	
	Inventory[Index].Amount -= Amount;
	FStoredItem RemoveItem = Inventory[Index];
	ClearInventory();
	OnItemRemoved.Broadcast(RemoveItem);
}

void UXYXInventoryManagerComponent::DropItem(FStoredItem Item)
{

}

void UXYXInventoryManagerComponent::UseItem(FGuid ItemId)
{
	int32 Index = FindIndexById(ItemId);
	if (IsSlotEmpty(Index))
	{
		return;
	}

	UXYXItemBase* ItemBase = NewObject<UXYXItemBase>(this, Inventory[Index].ItemClass);
	if (ItemBase)
	{
		ItemBase->UseItem(GetOwner());
	}

	if (Inventory[Index].ItemClass.GetDefaultObject()->Item.bIsConsumable)
	{
		RemoveItemAtIndex(Index, 1);
	}
}

bool UXYXInventoryManagerComponent::IsSlotEmpty(int32 Index)
{
	if (Inventory.IsValidIndex(Index))
	{
		return !IsItemValid(Inventory[Index]);
	}

	return true;
}

FStoredItem UXYXInventoryManagerComponent::GetItemAtIndex(int32 Index)
{
	FStoredItem Item;
	if (!IsSlotEmpty(Index))
	{
		Item = Inventory[Index];
	}
	
	return Item;
}

int32 UXYXInventoryManagerComponent::FindIndexByClass(TSubclassOf<class UXYXItemBase> ItemClass)
{
	int32 Index = -1;

	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i].ItemClass == ItemClass && Inventory[i].Amount >= 1)
		{
			Index = i;
			break;
		}
	}

	return Index;
}

int32 UXYXInventoryManagerComponent::FindIndexById(FGuid Id)
{
	int32  Index = -1;
	for (int32 i = 0; i<Inventory.Num(); ++i)
	{
		if (Inventory[i].Id == Id && Inventory[i].Amount >= 1)
		{
			Index = i;
			break;
		}
	}

	return Index;
}

bool UXYXInventoryManagerComponent::IsItemValid(FStoredItem Item)
{
	return Item.Id.IsValid() && UKismetSystemLibrary::IsValidClass(Item.ItemClass) && Item.Amount > 0;
}

void UXYXInventoryManagerComponent::HandleOnGameLoaded()
{
	auto&& XYXGameMode = Cast<AXYXGameMode>(UGameplayStatics::GetGameMode(this));
	if (IsValid(XYXGameMode))
	{
		Inventory = XYXGameMode->Inventory;
		ClearInventory();
	}
}


