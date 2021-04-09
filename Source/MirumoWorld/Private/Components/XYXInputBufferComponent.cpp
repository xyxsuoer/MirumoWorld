// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXInputBufferComponent.h"
#include "Game/XYXData.h"

// Sets default values for this component's properties
UXYXInputBufferComponent::UXYXInputBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UXYXInputBufferComponent::ComsumeInputBuffer()
{
	OnInputBufferConsumed.Broadcast(StoredKey);
	StoredKey = EInputBufferKey::ENone;
}

void UXYXInputBufferComponent::UpdateKey(EInputBufferKey Key)
{
	StoredKey = Key;
	if (!bIsBufferOpen)
		ComsumeInputBuffer();
}

void UXYXInputBufferComponent::OpenInputBuffer()
{
	bIsBufferOpen = true;
	OnInputBufferOpen.Broadcast();
}

void UXYXInputBufferComponent::CloseInputBuffer()
{
	bIsBufferOpen = false;
	OnInputBufferClose.Broadcast();
}

