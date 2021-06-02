// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/XYXData.h"
#include "XYXExtendedStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, float, NewValue, float, MaxValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXExtendedStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXExtendedStatComponent();

	UPROPERTY(BlueprintAssignable)
		FOnValueChanged OnValueChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EAttributesType AttributesType;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float RegenerationTickInterval = 0.05f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float CurrentValue = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float TopValue = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float ModifierValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bDoesRegenerates = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float RegenValue = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float ReenableRegenTime = 0.5f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FTimerHandle RegenTimer;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float InitialRegenValue = 0.f;

	UFUNCTION()
		void RegenTick();

	UFUNCTION()
		void ClearRegenTimer();

	UFUNCTION()
		void ChangeRegenPercent(int32 Percent);

	UFUNCTION()
		void ModifyStat(float Value, bool bInterruptRegeneration);
		
	UFUNCTION()
		float GetCurrentValue();

	UFUNCTION()
		float GetMaxValue();

	UFUNCTION()
		void AddModifier(float Value);

	UFUNCTION()
		void RemoveModifer(float Value);

	UFUNCTION()
		void SetCurrentValue(float Value, bool bInterruptRegeneration);

	UFUNCTION()
		void RefreshRegenTimer();

	UFUNCTION()
		void StartRegenerating();

	UFUNCTION()
		void HandleOnModifierAdded(EAttributesType Type, float Value);

	UFUNCTION()
		void HandleOnModifierRemoved(EAttributesType Type, float Value);

	UFUNCTION()
		void InitStatManager();

	UFUNCTION()
		void HandleOnGameLoaded();

	UFUNCTION()
		void HandleOnBaseValueChanged(EAttributesType Type, float Value);
};
