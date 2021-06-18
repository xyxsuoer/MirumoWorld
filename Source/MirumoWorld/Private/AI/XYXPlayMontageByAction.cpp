// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXPlayMontageByAction.h"
#include <Actors/XYXBaseNPC.h>
#include <Actors/XYXBaseAIController.h>
#include "Components/XYXMontageManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"

UXYXPlayMontageByAction::UXYXPlayMontageByAction()
{

}

void UXYXPlayMontageByAction::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	auto ControlledCharacter = Cast<AXYXBaseNPC>(ControlledPawn);
	auto AIController = Cast<AXYXBaseAIController>(OwnerController);

	if (!ControlledCharacter || !AIController)
	{
		FinishExecute(false);
		return;
	}

	auto MontagesManagerComp = ControlledCharacter->GetMontageManagerComponent();
	if (!MontagesManagerComp)
	{
		FinishExecute(false);
		return;
	}

	int32 TmpIndex = MontagesManagerComp->GetMontageActionLastIndex(CurAction);
	TmpIndex = UKismetMathLibrary::RandomIntegerInRange(0, TmpIndex);
	TmpIndex = bRandomIndex ? TmpIndex : CurIndex;

	UAnimMontage* Montage = MontagesManagerComp->GetMontageForAction(CurAction, TmpIndex);
	if (Montage)
	{
		if (ControlledCharacter->GetMesh() && ControlledCharacter->GetMesh()->GetAnimInstance())
		{
			float Duration = ControlledCharacter->GetMesh()->GetAnimInstance()->Montage_Play(Montage, PlayRate, EMontagePlayReturnType::Duration);
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
	}
	else
	{
		FinishExecute(false);
	}
}
