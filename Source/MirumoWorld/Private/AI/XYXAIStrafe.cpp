// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXAIStrafe.h"
#include <Actors/XYXBaseNPC.h>
#include "Actors/XYXBaseAIController.h"
#include "Components/XYXRotatingComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"

UXYXAIStrafe::UXYXAIStrafe()
{
	bCreateNodeInstance = true;
}

void UXYXAIStrafe::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

void UXYXAIStrafe::ReceiveTickAI(AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds)
{
	Strafe();
}

void UXYXAIStrafe::ReceiveActivationAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	ControlledCharacter = Cast<AXYXBaseNPC>(ControlledPawn);
	AIController = Cast<AXYXBaseAIController>(OwnerController);

	AActor* NewFocus = UBTFunctionLibrary::GetBlackboardValueAsActor(this, TargetKey);
	if (AIController)
	{
		AIController->SetFocus(NewFocus);
	}

	if (ControlledCharacter && ControlledCharacter->GetRotatingComponent())
	{
		ControlledCharacter->GetRotatingComponent()->SetRotationMode(ERotationMode::EFaceCamera);
		Strafe();
	}
}

void UXYXAIStrafe::ReceiveDeactivationAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
	OwnerController->StopMovement();

	auto TargetNPC = Cast<AXYXBaseNPC>(ControlledPawn);
	if (TargetNPC && TargetNPC->GetRotatingComponent())
	{
		TargetNPC->GetRotatingComponent()->SetRotationMode(ERotationMode::EOrientToMovement);
	}
}

UEnvQuery* UXYXAIStrafe::GetStrafeQuery()
{
	return nullptr;
}

void UXYXAIStrafe::Strafe()
{

}

void UXYXAIStrafe::HandleOnQueryFinished()
{

}
