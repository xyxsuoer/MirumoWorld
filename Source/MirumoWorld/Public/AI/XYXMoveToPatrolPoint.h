// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "XYXMoveToPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXMoveToPatrolPoint : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		class UXYXPatrolComponent* PatrolComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float AcceptanceRadius = 100.f;

public:

	UXYXMoveToPatrolPoint();

	void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);

	void ReceiveAbortAI(AAIController* OwnerController, APawn* ControlledPawn);
	
};
