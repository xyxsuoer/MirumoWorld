// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXMeleeAttack.h"
#include "Actors/XYXBaseNPC.h"
#include "Actors/XYXBaseAIController.h"

UXYXMeleeAttack::UXYXMeleeAttack()
{

}

void UXYXMeleeAttack::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	auto ControlledCharacter = Cast<AXYXBaseNPC>(ControlledPawn);
	auto AIController = Cast<AXYXBaseAIController>(OwnerController);

	if (!ControlledCharacter || !AIController)
	{
		FinishExecute(false);
		return;
	}

	float Duration = ControlledCharacter->MeleeAttack(AttackType);

	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([=]
		{
			FinishExecute(true);
		}
	);

	UWorld* World = GetWorld();
	check(World);
	FTimerHandle TmpTimer;
	World->GetTimerManager().SetTimer(TmpTimer, TimerCallback, Duration, false);

}
