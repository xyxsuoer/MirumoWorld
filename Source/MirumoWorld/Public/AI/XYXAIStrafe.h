// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "XYXAIStrafe.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXAIStrafe : public UBTService
{
	GENERATED_BODY()

public:

	UXYXAIStrafe();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
		UEnvQuery* GetStrafeQuery();

	UFUNCTION()
		void Strafe();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = XYX)
		UEnvQuery* QueryStrafeLeft;

	UPROPERTY(EditAnywhere, Category = XYX)
		UEnvQuery* QueryStrafeRight;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		class AXYXBaseNPC* ControlledCharacter;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		class AXYXBaseAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bUseRandomDirection = false;

};
