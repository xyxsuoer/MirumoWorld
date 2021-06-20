// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "XYXAIStrafe.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXAIStrafe : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:

	UXYXAIStrafe();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void ReceiveTickAI(AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds);

	void ReceiveActivationAI(AAIController* OwnerController, APawn* ControlledPawn);

	void ReceiveDeactivationAI(AAIController* OwnerController, APawn* ControlledPawn);

	UFUNCTION()
		UEnvQuery* GetStrafeQuery();

	UFUNCTION()
		void Strafe();

	UFUNCTION()
		void HandleOnQueryFinished();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FBlackboardKeySelector TargetKey;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		UEnvQuery* QueryTemplate;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		class AXYXBaseNPC* ControlledCharacter;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		class AXYXBaseAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bUseRandomDirection = false;
};
