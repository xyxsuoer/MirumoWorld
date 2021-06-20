// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXSetMovementState.h"
#include <Actors/XYXBaseNPC.h>
#include "Actors/XYXBaseAIController.h"
#include "Components/XYXMovementSpeedComponent.h"

UXYXSetMovementState::UXYXSetMovementState()
{

}

void UXYXSetMovementState::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	auto ControlledCharacter = Cast<AXYXBaseNPC>(ControlledPawn);
	auto AIController = Cast<AXYXBaseAIController>(OwnerController);

	if (!ControlledCharacter || !AIController)
	{
		FinishExecute(true);
		return;
	}

	auto MovementSpeedComp = ControlledCharacter->GetMovementSpeedComponent();
	if (MovementSpeedComp)
	{
		MovementSpeedComp->SetMovementState(MovementState);
	}

	FinishExecute(true);
}
