// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXAIStrafe.h"
#include <Actors/XYXBaseNPC.h>
#include "Actors/XYXBaseAIController.h"
#include "Components/XYXRotatingComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Kismet/KismetMathLibrary.h"

UXYXAIStrafe::UXYXAIStrafe()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void UXYXAIStrafe::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	AIController = Cast<AXYXBaseAIController>(OwnerComp.GetAIOwner());
	if (AIController)
		ControlledCharacter = Cast<AXYXBaseNPC>(AIController->PossesedAI);

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
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	AIController = Cast<AXYXBaseAIController>(OwnerComp.GetAIOwner());
	if (AIController)
		ControlledCharacter = Cast<AXYXBaseNPC>(AIController->PossesedAI);

	if (AIController) 
	{
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

UEnvQuery* UXYXAIStrafe::GetStrafeQuery()
{
	UEnvQuery* EnvQueryData = nullptr;
	
	if (!AIController || !AIController->GetBlackboardComponent())
	{
		return nullptr;
	}

	AActor* TargetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!TargetActor || !ControlledCharacter)
	{
		return nullptr;
	}

	FVector TargetRightVector = TargetActor->GetActorRightVector();
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector ControlledCharacterLocation = ControlledCharacter->GetActorLocation();
	FVector TmpVector = UKismetMathLibrary::GetDirectionUnitVector(TargetLocation, ControlledCharacterLocation);

	float TmpValue = UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Normal(
		FVector(TargetRightVector.X, TargetRightVector.Y, 0.f), 0.0001f),
		UKismetMathLibrary::Normal(FVector(TmpVector.X, TmpVector.Y, 0.f), 0.0001f));

	if (TmpValue >= -0.01)
	{
		EnvQueryData = QueryStrafeLeft;
	}
	else
	{
		EnvQueryData = QueryStrafeRight;
	}

	return EnvQueryData;
}

void UXYXAIStrafe::Strafe()
{
	UEnvQuery* TmpQuery = bUseRandomDirection ? 
		(UKismetMathLibrary::RandomBoolWithWeight(0.5f) ? QueryStrafeLeft : QueryStrafeRight) 
		: GetStrafeQuery();

	if (ControlledCharacter)
	{
		ControlledCharacter->SetMyQuery(TmpQuery);
		ControlledCharacter->RunEQS();
	}
}

