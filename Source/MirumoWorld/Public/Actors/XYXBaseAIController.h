// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "XYXBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API AXYXBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	AXYXBaseAIController();

protected:

		virtual void OnPossess(APawn* InPawn) override;

		virtual void OnUnPossess() override;

		virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ACF)
		class UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ACF)
		class UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		FName TargetKey = TEXT("Target");

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		FName AttackTypeKey = TEXT("AttackType");

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		FName StateKey = TEXT("State");

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		FName IsInCombatKey = TEXT("IsInCombat");

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		AActor* Target;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		class AXYXBaseNPC* PossesedAI;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		bool bIsInCombat = false;

	UPROPERTY()
		FTimerHandle UpdateTargetTimer;

	UFUNCTION()
		void HandleOnInCombatChanged(bool bValue);

	UFUNCTION()
		void UpdateTarget();

	UFUNCTION()
		void SetTarget(AActor* NewTarget);

	UFUNCTION()
		void SetIsInCombat(bool bValue);

	UFUNCTION()
		void UpdateSenseTarget();

};
