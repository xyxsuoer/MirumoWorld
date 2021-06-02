// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXExtendedStatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/XYXStatsManagerComponent.h"
#include <Game/XYXGameMode.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UXYXExtendedStatComponent::UXYXExtendedStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UXYXExtendedStatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitStatManager();
	InitialRegenValue = RegenValue;
	SetCurrentValue(TopValue, false);
	RefreshRegenTimer();
	auto&& Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto&& Owner = Cast<ACharacter>(GetOwner());
	if (Owner == Character)
	{
		auto&& XYXGameMode = Cast<AXYXGameMode>(UGameplayStatics::GetGameMode(this));
		if (IsValid(XYXGameMode))
		{
			XYXGameMode->OnGameLoaded.AddDynamic(this, &UXYXExtendedStatComponent::HandleOnGameLoaded);
		}
	}
}


void UXYXExtendedStatComponent::RegenTick()
{
	if (CurrentValue < GetMaxValue())
	{
		ModifyStat(RegenValue, false);
	}
	else
	{
		ClearRegenTimer();
	}
}

void UXYXExtendedStatComponent::ClearRegenTimer()
{
	UWorld* World = GetWorld();
	check(World);

	World->GetTimerManager().ClearTimer(RegenTimer);
}

void UXYXExtendedStatComponent::ChangeRegenPercent(int32 Percent)
{
	float TmpValue = Percent / 100.f;
	RegenValue = InitialRegenValue * TmpValue;
}

void UXYXExtendedStatComponent::ModifyStat(float Value, bool bInterruptRegeneration)
{
	if (Value == 0.f)
	{
		return;
	}

	if (bInterruptRegeneration)
	{
		ClearRegenTimer();
	}

	SetCurrentValue(CurrentValue + Value, false);
	RefreshRegenTimer();
}

float UXYXExtendedStatComponent::GetCurrentValue()
{
	return CurrentValue;
}

float UXYXExtendedStatComponent::GetMaxValue()
{
	return TopValue + ModifierValue;
}

void UXYXExtendedStatComponent::AddModifier(float Value)
{
	ModifierValue += Value;
	float TmpValue = UKismetMathLibrary::FClamp(CurrentValue, 0.f, GetMaxValue());
	SetCurrentValue(TmpValue, false);
	RefreshRegenTimer();
}

void UXYXExtendedStatComponent::RemoveModifer(float Value)
{
	ModifierValue -= Value;
	float TmpValue = UKismetMathLibrary::FClamp(CurrentValue, 0.f, GetMaxValue());
	SetCurrentValue(TmpValue, false);
	RefreshRegenTimer();
}

void UXYXExtendedStatComponent::SetCurrentValue(float Value, bool bInterruptRegeneration)
{
	CurrentValue = UKismetMathLibrary::FClamp(Value, -100.f, GetMaxValue());
	OnValueChanged.Broadcast(CurrentValue, GetMaxValue());

	if (bInterruptRegeneration)
	{
		ClearRegenTimer();
		RefreshRegenTimer();
	}
}

void UXYXExtendedStatComponent::RefreshRegenTimer()
{
	if (!bDoesRegenerates)
	{
		return;
	}
	UWorld* World = GetWorld();
	check(World);
	if (!World->GetTimerManager().IsTimerActive(RegenTimer))
	{
		World->GetTimerManager().SetTimer(RegenTimer, this, &UXYXExtendedStatComponent::StartRegenerating, ReenableRegenTime, false);
	}
}

void UXYXExtendedStatComponent::StartRegenerating()
{
	UWorld* World = GetWorld();
	check(World);

	World->GetTimerManager().SetTimer(RegenTimer, this, &UXYXExtendedStatComponent::RegenTick, RegenerationTickInterval, true);
	
}

void UXYXExtendedStatComponent::HandleOnModifierAdded(EAttributesType Type, float Value)
{
	if (AttributesType == Type)
	{
		AddModifier(Value);
	}
}

void UXYXExtendedStatComponent::HandleOnModifierRemoved(EAttributesType Type, float Value)
{
	if (AttributesType == Type)
	{
		RemoveModifer(Value);
	}
}

void UXYXExtendedStatComponent::InitStatManager()
{
	auto StatsComp = Cast<UXYXStatsManagerComponent>(GetOwner()->GetComponentByClass(UXYXStatsManagerComponent::StaticClass()));
	if (StatsComp)
	{
		TopValue = StatsComp->GetStatValue(AttributesType, false);
		StatsComp->OnModifierAdded.AddDynamic(this, &UXYXExtendedStatComponent::HandleOnModifierAdded);
		StatsComp->OnModifierRemoved.AddDynamic(this, &UXYXExtendedStatComponent::HandleOnModifierRemoved);
		StatsComp->OnBaseValueChanged.AddDynamic(this, &UXYXExtendedStatComponent::HandleOnBaseValueChanged);
	}
}

void UXYXExtendedStatComponent::HandleOnGameLoaded()
{
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([=]
		{
			auto&& XYXGameMode = Cast<AXYXGameMode>(UGameplayStatics::GetGameMode(this));
			if (IsValid(XYXGameMode))
			{
				if (XYXGameMode->StatValues.Contains(AttributesType))
				{
					SetCurrentValue(XYXGameMode->StatValues[AttributesType], false);
				}
			}
		}
	);

	UWorld* World = GetWorld();
	check(World);
	FTimerHandle TmpTimer;
	World->GetTimerManager().SetTimer(TmpTimer, TimerCallback, 0.001f, false);
}

void UXYXExtendedStatComponent::HandleOnBaseValueChanged(EAttributesType Type, float Value)
{
	if (AttributesType == Type)
	{
		TopValue = Value;
		OnValueChanged.Broadcast(CurrentValue, GetMaxValue());
	}
}

