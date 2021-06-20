// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Game/XYXData.h"
#include "XYXMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXMeleeAttack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()


public:

	UXYXMeleeAttack();

	void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EMeleeAttackType AttackType = EMeleeAttackType::ELight;

	
};
