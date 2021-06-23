// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/XYXEQSContextStrafeLeft.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Actors/XYXBaseAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "AIHelpers.h"

UXYXEQSContextStrafeLeft::UXYXEQSContextStrafeLeft()
{

}

void UXYXEQSContextStrafeLeft::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
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
		FVector TargetLocation = AIController->GetSeeingPawn()->GetActorLocation();
		FVector QuerierActorLocation = QuerierActor->GetActorLocation();
		FRotator  TmpRotator = UKismetMathLibrary::FindLookAtRotation(QuerierActorLocation, TargetLocation);
		FRotator NewRotator = FRotator(0.f, 0.f, TmpRotator.Yaw - 90.f);
		FVector ResultingLocation = UKismetMathLibrary::Conv_RotatorToVector(NewRotator) * 300.f + QuerierActorLocation;
		UEnvQueryItemType_Point::SetContextHelper(ContextData, ResultingLocation);
	}
}