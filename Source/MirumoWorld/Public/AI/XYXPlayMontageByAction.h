// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Game/XYXData.h"
#include "XYXPlayMontageByAction.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXPlayMontageByAction : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UXYXPlayMontageByAction();

	void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EMontageAction CurAction = EMontageAction::ENone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		int32 CurIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float PlayRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bRandomIndex = false;
};
