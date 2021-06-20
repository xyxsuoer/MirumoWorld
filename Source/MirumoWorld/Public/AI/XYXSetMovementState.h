// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Game/XYXData.h"
#include "XYXSetMovementState.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXSetMovementState : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UXYXSetMovementState();

	void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EMovementState MovementState = EMovementState::EJog;
	
};
