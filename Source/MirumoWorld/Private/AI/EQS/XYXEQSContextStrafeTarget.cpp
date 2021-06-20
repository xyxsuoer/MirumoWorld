// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/XYXEQSContextStrafeTarget.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Actors/XYXBaseAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_BlueprintBase.h"
#include "AIHelpers.h"


UXYXEQSContextStrafeTarget::UXYXEQSContextStrafeTarget()
{

}

UWorld* UXYXEQSContextStrafeTarget::GetWorld() const
{
	UEnvQueryManager* EnvQueryManager = Cast<UEnvQueryManager>(GetOuter());
	if (EnvQueryManager)
	{
		return EnvQueryManager->GetWorld();
	}

	return nullptr;
}

void UXYXEQSContextStrafeTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* QuerierObject = QueryInstance.Owner.Get();
	if ((QuerierObject == nullptr) || (CallMode == InvalidCallMode))
	{
		return;
	}

	// NOTE: QuerierActor is redundant with QuerierObject and should be removed in the future.  It's here for now for
	// backwards compatibility.
	AActor* QuerierActor = Cast<AActor>(QuerierObject);
	AActor* ResultingActor = NULL;
	ProvideSingleActor(QuerierObject, QuerierActor, ResultingActor);
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, ResultingActor);
	
}

void UXYXEQSContextStrafeTarget::ProvideSingleActor(UObject* QuerierObject, AActor* QuerierActor, AActor*& ResultingActor) const
{
	auto AIController = Cast<AXYXBaseAIController>(UAIBlueprintHelperLibrary::GetAIController(QuerierActor));
	if (AIController && AIController->Target)
	{
		ResultingActor = AIController->Target;
	}

	ResultingActor = QuerierActor;
}
