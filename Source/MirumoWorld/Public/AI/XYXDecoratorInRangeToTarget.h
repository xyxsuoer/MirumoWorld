// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "XYXDecoratorInRangeToTarget.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXDecoratorInRangeToTarget : public UBTDecorator_BlueprintBase
{
	GENERATED_BODY()

public:

	UXYXDecoratorInRangeToTarget();

	bool PerformConditionCheckAI(AAIController* OwnerController, APawn* ControlledPawn);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float GreaterThan = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float LessThan = 300.f;
	
};
