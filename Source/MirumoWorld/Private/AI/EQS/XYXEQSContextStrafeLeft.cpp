// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/XYXEQSContextStrafeLeft.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Actors/XYXBaseAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_BlueprintBase.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "AIHelpers.h"

UXYXEQSContextStrafeLeft::UXYXEQSContextStrafeLeft()
{

}

UWorld* UXYXEQSContextStrafeLeft::GetWorld() const
{
	UEnvQueryManager* EnvQueryManager = Cast<UEnvQueryManager>(GetOuter());
	if (EnvQueryManager)
	{
		return EnvQueryManager->GetWorld();
	}

	return nullptr;
}

void UXYXEQSContextStrafeLeft::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* QuerierObject = QueryInstance.Owner.Get();
	if ((QuerierObject == nullptr) || (CallMode == InvalidCallMode))
	{
		return;
	}

	// NOTE: QuerierActor is redundant with QuerierObject and should be removed in the future.  It's here for now for
	// backwards compatibility.
	AActor* QuerierActor = Cast<AActor>(QuerierObject);
	FVector ResultingLocation = FAISystem::InvalidLocation;
	ProvideSingleLocation(QuerierObject, QuerierActor, ResultingLocation);
	UEnvQueryItemType_Point::SetContextHelper(ContextData, ResultingLocation);
}

void UXYXEQSContextStrafeLeft::ProvideSingleLocation(UObject* QuerierObject, AActor* QuerierActor, FVector& ResultingLocation) const
{
	auto AIController = Cast<AXYXBaseAIController>(UAIBlueprintHelperLibrary::GetAIController(QuerierActor));
	if (QuerierActor && AIController && AIController->Target)
	{
		FVector TargetLocation = AIController->Target->GetActorLocation();
		FVector QuerierActorLocation = QuerierActor->GetActorLocation();
		FRotator  TmpRotator =  UKismetMathLibrary::FindLookAtRotation(QuerierActorLocation, TargetLocation);
		FRotator NewRotator = FRotator(0.f, 0.f, TmpRotator.Yaw - 90.f);
		ResultingLocation = UKismetMathLibrary::Conv_RotatorToVector(NewRotator) * 300.f + QuerierActorLocation;
	}
}
