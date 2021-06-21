// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "XYXUpdateMeleeAIBehavior.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXUpdateMeleeAIBehavior : public UBTService
{
	GENERATED_BODY()

public:

	UXYXUpdateMeleeAIBehavior();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FBlackboardKeySelector BehaviorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FBlackboardKeySelector TargetKey;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class AXYXBaseAIController* AIController;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class AXYXBaseNPC* ControlledCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float AttackBehaviorRange = 500.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float TicksNearTarget = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsOutOfStamina = false;

public:

	UFUNCTION()
		void Update();

	UFUNCTION()
		void UpdateBehavior();

	UFUNCTION()
		void UpdateActivities();

	UFUNCTION()
		void SetBehavior(EAIBehavior Behavior);

	UFUNCTION()
		void HandleOnStateChanged(EState PrevState, EState NewState);

	UFUNCTION()
		void HandleOnStaminaValueChanged(float NewValue, float MaxValue);
	
};
