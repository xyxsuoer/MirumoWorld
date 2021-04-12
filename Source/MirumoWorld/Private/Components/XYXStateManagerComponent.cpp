// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXStateManagerComponent.h"

// Sets default values for this component's properties
UXYXStateManagerComponent::UXYXStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


void UXYXStateManagerComponent::SetState(EState State)
{
	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(SetIdleStateTimer);
	EState PrevState = CurrentState;
	CurrentState = State;
	if (CurrentState != PrevState)
	{
		OnStateChanged.Broadcast(PrevState, CurrentState);
	}
}

void UXYXStateManagerComponent::SetActivity(EActivity Activity, bool Value)
{
	if (Value != GetActivityValue(Activity))
	{
		Activities.Add(Activity, Value);
		OnActivityChanged.Broadcast(Activity, Value);
	}
}

bool UXYXStateManagerComponent::GetActivityValue(EActivity Activity)
{
	if (Activities.Contains(Activity))
	{
		return Activities[Activity];
	}
	else
	{
		return false;
	}
}

void UXYXStateManagerComponent::ResetState(float InTime)
{
	if (CurrentState == EState::EDead)
	{
		return;
	}

	if (InTime <= 0.f)
	{
		SetIdleState();
	}
	else
	{
		UWorld* World = GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(SetIdleStateTimer, this, &UXYXStateManagerComponent::SetIdleState, InTime, false);
	}
}

void UXYXStateManagerComponent::SetIdleState()
{
	SetState(EState::EIdle);
}
