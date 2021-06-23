// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/XYXEQSContextStrafeTarget.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Actors/XYXBaseAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AIHelpers.h"


UXYXEQSContextStrafeTarget::UXYXEQSContextStrafeTarget()
{

}

void UXYXEQSContextStrafeTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	UObject* QuerierObject = QueryInstance.Owner.Get();
	AActor* QuerierActor = Cast<AActor>(QuerierObject);
	if (!QuerierObject || !QuerierActor)
	{
		return;
	}

	AXYXBaseAIController* AIController = Cast<AXYXBaseAIController>(QuerierActor->GetInstigatorController());
	if (AIController && AIController->GetSeeingPawn())
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, AIController->GetSeeingPawn());
	}
	else
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, QuerierActor);
	}
}
