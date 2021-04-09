// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XYXInputBufferComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputBufferConsumed, EInputBufferKey, Key);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputBufferOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputBufferClose);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXInputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXInputBufferComponent();

	UPROPERTY(BlueprintAssignable)
		FOnInputBufferConsumed OnInputBufferConsumed;

	UPROPERTY(BlueprintAssignable)
		FOnInputBufferOpen OnInputBufferOpen;

	UPROPERTY(BlueprintAssignable)
		FOnInputBufferClose OnInputBufferClose;

public:	

	UFUNCTION(BlueprintCallable, Category = XYX)
		EInputBufferKey GetStoredKey() { return StoredKey; }

	UFUNCTION(BlueprintCallable, Category = XYX)
		void ComsumeInputBuffer();

	UFUNCTION(BlueprintCallable, Category = XYX)
		void UpdateKey(EInputBufferKey Key);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void OpenInputBuffer();

	UFUNCTION(BlueprintCallable, Category = XYX)
		void CloseInputBuffer();

private:

	UPROPERTY()
		bool bIsBufferOpen = false;

	UPROPERTY()
		EInputBufferKey StoredKey;

		
};
