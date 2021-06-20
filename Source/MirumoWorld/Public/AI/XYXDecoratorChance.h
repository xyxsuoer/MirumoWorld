// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "XYXDecoratorChance.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXDecoratorChance : public UBTDecorator_BlueprintBase
{
	GENERATED_BODY()

public:

	UXYXDecoratorChance();

	bool PerformConditionCheckAI(AAIController* OwnerController, APawn* ControlledPawn);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		int32 Chance = 0;
	
};
