// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXStatsManagerComponent.h"
#include "Interfaces/XYXInterfaceItem.h"
#include "Items/ObjectItems/XYXItemBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/XYXEquipmentManagerComponent.h"

// Sets default values for this component's properties
UXYXStatsManagerComponent::UXYXStatsManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UXYXStatsManagerComponent::ExcludeItemModifiers(TSubclassOf<class UXYXItemBase> Item)
{
	if (UKismetSystemLibrary::IsValidClass(Item))
	{
		UXYXItemBase* ItemBase = NewObject<UXYXItemBase>(this, Item);
		if (ItemBase)
		{
			IXYXInterfaceItem* MyInterface = Cast<IXYXInterfaceItem>(ItemBase);
			if (MyInterface)
			{
				auto&& TmpVec = MyInterface->GetModifiers();
				for (auto && e : TmpVec)
				{
					RemoveModifier(e.Type, e.Value);
				}
			}
		}
	}
}

void UXYXStatsManagerComponent::IncludeItemModifiers(TSubclassOf<class UXYXItemBase> Item)
{
	if (UKismetSystemLibrary::IsValidClass(Item))
	{
		UXYXItemBase* ItemBase = NewObject<UXYXItemBase>(this, Item);
		if (ItemBase)
		{
			IXYXInterfaceItem* MyInterface = Cast<IXYXInterfaceItem>(ItemBase);
			if (MyInterface)
			{
				auto&& TmpVec = MyInterface->GetModifiers();
				for (auto&& e : TmpVec)
				{
					AddModifier(e.Type, e.Value);
				}
			}
		}
	}
}

void UXYXStatsManagerComponent::AddModifier(EAttributesType Type, float Value)
{
	if (Value == 0.f)
	{
		return;
	}

	int32 TmpIndex = GetStatIndex(Type);
	if (TmpIndex >= 0)
	{
		if (Stats.IsValidIndex(TmpIndex))
		{
			Stats[TmpIndex].ModifiersValue += Value;
			OnModifierAdded.Broadcast(Type, Value);
		}
	}

}

void UXYXStatsManagerComponent::RemoveModifier(EAttributesType Type, float Value)
{
	if (Value == 0.f)
	{
		return;
	}

	int32 TmpIndex = GetStatIndex(Type);
	if (TmpIndex >= 0)
	{
		if (Stats.IsValidIndex(TmpIndex))
		{
			Stats[TmpIndex].ModifiersValue -= Value;
			OnModifierRemoved.Broadcast(Type, Value);
		}
	}
}

float UXYXStatsManagerComponent::GetStatValue(EAttributesType Type, bool bIncludeModifiers)
{
	float Value = 0.f;
	int32 TmpIndex = GetStatIndex(Type);
	if (TmpIndex >= 0)
	{
		if (Stats.IsValidIndex(TmpIndex))
		{
			if (bIncludeModifiers)
			{
				Value = Stats[TmpIndex].BaseValue + Stats[TmpIndex].ModifiersValue;
			}
			else
			{
				Value = Stats[TmpIndex].BaseValue;
			}
		}
	}

	return Value;
}

int32 UXYXStatsManagerComponent::GetStatIndex(EAttributesType Type)
{
	int32 Index = -1;

	for (int32 i = 0; i< Stats.Num(); ++i)
	{
		if (Stats[i].Type == Type)
		{
			Index = i;
			break;
		}
	}

	return Index;
}

float UXYXStatsManagerComponent::GetDamage()
{
	float RandomValue = UKismetMathLibrary::RandomFloatInRange(1.f, 100.f);
	float TmpValue = 1.0f;
	float Damage = 0.f;
	if (RandomValue <= GetStatValue(EAttributesType::ECritChance, true))
	{
		TmpValue = GetStatValue(EAttributesType::ECritMultiplier, true);
	}

	Damage = GetStatValue(EAttributesType::EDamage, true) * TmpValue;
	Damage = UKismetMathLibrary::FTrunc(Damage);

	return Damage;

}

void UXYXStatsManagerComponent::ChangeStatBaseValue(EAttributesType Type, float Value)
{
	int32 TmpIndex = GetStatIndex(Type);
	if (Stats.IsValidIndex(TmpIndex))
	{
		Stats[TmpIndex].BaseValue = Value;
		OnBaseValueChanged.Broadcast(Type, Value);
	}
}

void UXYXStatsManagerComponent::TakeDamage(float Damage, bool bIgnoreStamina)
{
	UWorld* World = GetWorld();
	check(World);

	World->GetTimerManager().ClearTimer(ResetRecentlyReceivedDamageTimer);
	RecentlyReceivedDamage += Damage;
	RecentlyReceivedHitsCount++;
	World->GetTimerManager().SetTimer(ResetRecentlyReceivedDamageTimer, this, &UXYXStatsManagerComponent::ResetRecentlyReceivedDamage, 4.f, false);

}

float UXYXStatsManagerComponent::GetRecentlyReceivedDamage()
{
	return RecentlyReceivedDamage;
}

void UXYXStatsManagerComponent::ResetRecentlyReceivedDamage()
{
	RecentlyReceivedDamage = 0.f;
	RecentlyReceivedHitsCount = 0;
	RecentlyReceivedSuccessfulDamage = 0.f;
	RecentlyReceivedSuccessfulHitsCount = 0;
}

int32 UXYXStatsManagerComponent::GetRecentlyReceivedHitsCount()
{
	return RecentlyReceivedHitsCount;
}

int32 UXYXStatsManagerComponent::GetRecentlyReceivedSuccessfulHitsCount()
{
	return RecentlyReceivedSuccessfulHitsCount;
}

float UXYXStatsManagerComponent::GetRecentlyReceivedSuccessfulDamage()
{
	return RecentlyReceivedSuccessfulDamage;
}

void UXYXStatsManagerComponent::UpdateBlockBaseValue()
{
	if (EquipmentComp)
	{
		float TmpValue = 0.f;
		bool TmpSuccess = false;
		EquipmentComp->GetBlockValue(TmpValue, TmpSuccess);
		float NewValue = InitialBlockValue;
		if (TmpSuccess)
		{
			NewValue = TmpValue;
		}

		ChangeStatBaseValue(EAttributesType::EBlock, NewValue);
	}
}

void UXYXStatsManagerComponent::Initialize()
{
	InitialBlockValue = GetStatValue(EAttributesType::EBlock, false);
	EquipmentComp = Cast<UXYXEquipmentManagerComponent>(GetOwner()->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));
	if (EquipmentComp)
	{
		EquipmentComp->OnActiveItemChanged.AddDynamic(this, &UXYXStatsManagerComponent::HandleOnActiveItemChanged);
		EquipmentComp->OnSlotHiddenChanged.AddDynamic(this, &UXYXStatsManagerComponent::HandleOnSlotHiddenChanged);
		EquipmentComp->OnMainHandTypeChanged.AddDynamic(this, &UXYXStatsManagerComponent::HandleOnMainHandTypeChanged);
	}

}

void UXYXStatsManagerComponent::HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex)
{
	UpdateBlockBaseValue();

	if (EquipmentComp && !EquipmentComp->IsSlotHidden(Type, SlotIndex))
	{
		IncludeItemModifiers(NewItem.ItemClass);
		ExcludeItemModifiers(OldItem.ItemClass);
	}
}

void UXYXStatsManagerComponent::HandleOnSlotHiddenChanged(EItemType SlotType, int32 SlotIndex, FStoredItem ActiveItem, bool bIsHidden)
{
	if (bIsHidden)
	{
		ExcludeItemModifiers(ActiveItem.ItemClass);
	}
	else
	{
		IncludeItemModifiers(ActiveItem.ItemClass);
	}
}

void UXYXStatsManagerComponent::HandleOnMainHandTypeChanged(EItemType Type)
{
	UpdateBlockBaseValue();
}

