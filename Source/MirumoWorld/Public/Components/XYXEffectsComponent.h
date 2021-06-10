// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/XYXData.h"
#include "XYXEffectsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectApplied, EEffectType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectRemoved, EEffectType, Type);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXEffectsComponent();

	UPROPERTY(BlueprintAssignable)
		FOnEffectApplied  OnEffectApplied;

	UPROPERTY(BlueprintAssignable)
		FOnEffectRemoved  OnEffectRemoved;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float BackstabDamage = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float BurningbDamage = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FEffect> AppliedEffects;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float UpdateInterval = 0.1f;

	UPROPERTY()
		FTimerHandle UpdateEffectsDurationTimer;

	UFUNCTION()
		FEffect GetEffect(EEffectType Type);

	UFUNCTION()
		int32 GetEffectIndex(EEffectType Type);

	UFUNCTION()
		AActor* GetEffectApplier(EEffectType Type);

	UFUNCTION()
		bool IsAnyEffectApplied(TArray<EEffectType>& Types);

	UFUNCTION()
		bool IsEffectApplied(EEffectType Type);

	UFUNCTION()
		bool ApplyEffect(EEffectType Type, float Duration, EApplyEffectMethod Method, AActor* Applier);

	UFUNCTION()
		void RemoveEffect(EEffectType Type);

	UFUNCTION()
		void AdjustEffectTime(EEffectType Type, float NewDuration);

	UFUNCTION()
		void UpdateEffectsDuration();

	UFUNCTION()
		void RemoveEffects(TArray<EEffectType> Types);

	UFUNCTION()
		bool ApplyBackstabEffect(float Duration, EApplyEffectMethod Method, AActor* Applier, float Damage);

	UFUNCTION()
		bool ApplyBurringEffect(float Duration, EApplyEffectMethod Method, AActor* Applier, float Damage);

	UFUNCTION()
		void UpdateEffect(EEffectType Type, float Duration, EApplyEffectMethod Method, AActor* Applier);

};
