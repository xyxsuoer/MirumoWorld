// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXAIStrafe.h"
#include <Actors/XYXBaseNPC.h>
#include "Actors/XYXBaseAIController.h"
#include "Components/XYXRotatingComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UXYXAIStrafe::UXYXAIStrafe()
{

}

void UXYXAIStrafe::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AIController)
	{
		auto NewFocus = Cast<AActor>(AIController->BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
		AIController->SetFocus(NewFocus);
	}

	if (ControlledCharacter && ControlledCharacter->GetRotatingComponent())
	{
		ControlledCharacter->GetRotatingComponent()->SetRotationMode(ERotationMode::EFaceCamera);
		Strafe();
	}
}

void UXYXAIStrafe::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AIController = Cast<AXYXBaseAIController>(OwnerComp.GetAIOwner());
	if (AIController) 
	{
		//ControlledCharacter = Cast<AXYXBaseNPC>(AIController->PossesedAI);

		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		AIController->StopMovement();

		if (ControlledCharacter && ControlledCharacter->GetRotatingComponent())
		{
			ControlledCharacter->GetRotatingComponent()->SetRotationMode(ERotationMode::EOrientToMovement);
		}
	}
}

void UXYXAIStrafe::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Strafe();
}

void UXYXAIStrafe::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	AIController = Cast<AXYXBaseAIController>(SearchData.OwnerComp.GetAIOwner());
	if (AIController)
		ControlledCharacter = Cast<AXYXBaseNPC>(AIController->PossesedAI);
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
