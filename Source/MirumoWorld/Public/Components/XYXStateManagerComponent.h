// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/XYXData.h"
#include "XYXStateManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChanged, EState, PrevState, EState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActivityChanged, EActivity, Activity, bool, Value);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXStateManagerComponent();

	UPROPERTY(BlueprintAssignable)
		FOnStateChanged OnStateChanged;

	UPROPERTY(BlueprintAssignable)
		FOnActivityChanged OnActivityChanged;

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SetState(EState State);

	UFUNCTION(BlueprintCallable, Category = XYX)
		EState GetState(){ return CurrentState; }

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SetActivity(EActivity Activity, bool Value);

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool GetActivityValue(EActivity Activity);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void ResetState(float InTime);

	UFUNCTION(BlueprintCallable, Category = XYX)
		void SetIdleState();

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TMap<EActivity, bool> Activities;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EState CurrentState = EState::EIdle;

	UPROPERTY()
		FTimerHandle SetIdleStateTimer;
		
};
