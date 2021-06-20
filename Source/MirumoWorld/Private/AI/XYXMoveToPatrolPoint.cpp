// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXMoveToPatrolPoint.h"
#include "Components/XYXPatrolComponent.h"
#include "Actors/XYXBaseNPC.h"
#include "Actors/XYXBaseAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UXYXMoveToPatrolPoint::UXYXMoveToPatrolPoint()
{

}

void UXYXMoveToPatrolPoint::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	auto ControlledCharacter = Cast<AXYXBaseNPC>(ControlledPawn);
	auto AIController = Cast<AXYXBaseAIController>(OwnerController);

	PatrolComp = ControlledCharacter->GetPatrolComponent();
	if (PatrolComp)
	{
		if (PatrolComp->PatrolPath)
		{
			FVector Destination = PatrolComp->GetSplinePointLocation(PatrolComp->PointIndex);
			UAIBlueprintHelperLibrary::CreateMoveToProxyObject(this, ControlledPawn, Destination, nullptr, AcceptanceRadius, true);
			FinishExecute(true);
		}
		else
		{
			FinishExecute(false);
		}
	}
	else
	{
		FinishExecute(false);
	}
}

void UXYXMoveToPatrolPoint::ReceiveAbortAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	auto ControlledCharacter = Cast<AXYXBaseNPC>(ControlledPawn);
	auto AIController = Cast<AXYXBaseAIController>(OwnerController);

	if (AIController)
	{
		AIController->StopMovement();
	}

	FinishAbort();
}
