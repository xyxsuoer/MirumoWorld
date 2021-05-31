// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXEffectsComponent.h"
#include <Interfaces/XYXInterfaceEntity.h>

// Sets default values for this component's properties
UXYXEffectsComponent::UXYXEffectsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UXYXEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


FEffect UXYXEffectsComponent::GetEffect(EEffectType Type)
{
	FEffect Result;
	for (auto&& e : AppliedEffects)
	{
		if (e.Type == Type)
		{
			Result = e;
			break;
		}
	}

	return Result;
}

int32 UXYXEffectsComponent::GetEffectIndex(EEffectType Type)
{
	int32 Index = -1;

	for (int32 i = 0; i< AppliedEffects.Num(); ++i)
	{
		if (AppliedEffects[i].Type == Type)
		{
			Index = i;
			break;
		}
	}

	return Index;
}

AActor* UXYXEffectsComponent::GetEffectApplier(EEffectType Type)
{
	FEffect TmpEffect = GetEffect(Type);
	return TmpEffect.Applier;
}

bool UXYXEffectsComponent::IsEffectApplied(EEffectType Type)
{
	int32 TmpIndex = GetEffectIndex(Type);

	return TmpIndex >= 0;
}

bool UXYXEffectsComponent::IsAnyEffectApplied(TArray<EEffectType>& Types)
{
	for (auto&& e: Types)
	{
		if (IsEffectApplied(e))
		{
			return true;
		}
	}

	return false;
}

bool UXYXEffectsComponent::ApplyEffect(EEffectType Type, float Duration, EApplyEffectMethod Method, AActor* Applier)
{
	IXYXInterfaceEntity* TmpEntity = Cast<IXYXInterfaceEntity>(GetOwner());
	if (TmpEntity)
	{
		if (!TmpEntity->CanEffectBeApplied(Type, Applier))
		{
			return false;
		}
	}

	UpdateEffect(Type, Duration, Method, Applier);
	OnEffectApplied.Broadcast(Type);
	return true;
}

void UXYXEffectsComponent::RemoveEffect(EEffectType Type)
{
	FEffect TmpEffect = GetEffect(Type);

	if (AppliedEffects.Contains(TmpEffect))
	{
		AppliedEffects.Remove(TmpEffect);
		OnEffectRemoved.Broadcast(Type);
	}
}

void UXYXEffectsComponent::AdjustEffectTime(EEffectType Type, float NewDuration)
{
	int32 TmpIndex = GetEffectIndex(Type);
	if (TmpIndex >= 0)
	{
		AppliedEffects[TmpIndex].Duration = NewDuration;
	}
}

void UXYXEffectsComponent::UpdateEffectsDuration()
{
	if (AppliedEffects.Num() <= 0)
	{
		return;
	}
	TArray<FEffect> TmpVec;
	for (int32 i = 0; i < AppliedEffects.Num(); ++i)
	{
		AppliedEffects[i].Duration -= UpdateInterval;
		if (AppliedEffects[i].Duration <= UpdateInterval)
		{
			TmpVec.Add(AppliedEffects[i]);
		}
	}

	for (auto&& e : TmpVec)
	{
		RemoveEffect(e.Type);
	}
}

void UXYXEffectsComponent::RemoveEffects(TArray<EEffectType> Types)
{
	for (auto&& e : Types)
	{
		RemoveEffect(e);
	}
}

bool UXYXEffectsComponent::ApplyBackstabEffect(float Duration, EApplyEffectMethod Method, AActor* Applier, float Damage)
{
	IXYXInterfaceEntity* TmpEntity = Cast<IXYXInterfaceEntity>(GetOwner());
	if (TmpEntity)
	{
		if (!TmpEntity->CanEffectBeApplied(EEffectType::EBackstab, Applier))
		{
			return false;
		}
	}

	BackstabDamage = Damage;
	UpdateEffect(EEffectType::EBackstab, Duration, Method, Applier);
	OnEffectApplied.Broadcast(EEffectType::EBackstab);
	return true;
}

bool UXYXEffectsComponent::ApplyBurringEffect(float Duration, EApplyEffectMethod Method, AActor* Applier, float Damage)
{
	IXYXInterfaceEntity* TmpEntity = Cast<IXYXInterfaceEntity>(GetOwner());
	if (TmpEntity)
	{
		if (!TmpEntity->CanEffectBeApplied(EEffectType::EBurning, Applier))
		{
			return false;
		}
	}

	BurningbDamage = Damage;
	UpdateEffect(EEffectType::EBurning,Duration, Method, Applier);
	OnEffectApplied.Broadcast(EEffectType::EBurning);
	return true;
}

void UXYXEffectsComponent::UpdateEffect(EEffectType Type, float Duration, EApplyEffectMethod Method, AActor* Applier)
{
	int32 TmpIndex = GetEffectIndex(Type);
	if (TmpIndex >= 0)
	{
		switch(Method)
		{
		case EApplyEffectMethod::EReplace: 
			{
				if (AppliedEffects.IsValidIndex(TmpIndex))
				{
					AppliedEffects[TmpIndex].Applier = Applier;
					AppliedEffects[TmpIndex].Duration = Duration;
				}
			}
			break;
		case EApplyEffectMethod::EStack:
			{
				if (AppliedEffects.IsValidIndex(TmpIndex))
				{
					AppliedEffects[TmpIndex].Applier = Applier;
					AppliedEffects[TmpIndex].Duration += Duration;
				}
			}
			break;
		}
	}
	else
	{
		FEffect TmpEffect;
		TmpEffect.Applier = Applier;
		TmpEffect.Duration = Duration;
		TmpEffect.Type = Type;
		AppliedEffects.Add(TmpEffect);
	}
}

