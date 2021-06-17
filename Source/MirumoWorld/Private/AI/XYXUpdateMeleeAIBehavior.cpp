// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXUpdateMeleeAIBehavior.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include <Actors/XYXBaseNPC.h>
#include "BehaviorTree/BTFunctionLibrary.h"
#include <Actors/XYXBaseAIController.h>
#include "Components/XYXStateManagerComponent.h"
#include "Components/XYXExtendedStatComponent.h"

UXYXUpdateMeleeAIBehavior::UXYXUpdateMeleeAIBehavior()
{
	bCreateNodeInstance = true;
}

void UXYXUpdateMeleeAIBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Update();
}

void UXYXUpdateMeleeAIBehavior::ReceiveSearchStartAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	ControlledCharacter = Cast<AXYXBaseNPC>(ControlledPawn);
	AIController = Cast<AXYXBaseAIController>(OwnerController);

	if (ControlledCharacter )
	{
		UXYXStateManagerComponent* StateComp = ControlledCharacter->GetStateManagerComponent();
		UXYXExtendedStatComponent* ExtendedStamina = ControlledCharacter->GetExtendedStamina();
		if (StateComp)
		{
			StateComp->OnStateChanged.AddDynamic(this, &UXYXUpdateMeleeAIBehavior::HandleOnStateChanged);
		}

		if (ExtendedStamina)
		{
			ExtendedStamina->OnValueChanged.AddDynamic(this, &UXYXUpdateMeleeAIBehavior::HandleOnStaminaValueChanged);
		}
	}

}

void UXYXUpdateMeleeAIBehavior::Update()
{
	UpdateBehavior();
	UpdateActivities();
}

void UXYXUpdateMeleeAIBehavior::UpdateBehavior()
{
	if (!AIController || !ControlledCharacter)
	{
		return;
	}

}

void UXYXUpdateMeleeAIBehavior::UpdateActivities()
{

}

void UXYXUpdateMeleeAIBehavior::SetBehavior(EAIBehavior Behavior)
{
	UBTFunctionLibrary::SetBlackboardValueAsEnum(this, BehaviorKey, (uint8)Behavior);
}

void UXYXUpdateMeleeAIBehavior::HandleOnStateChanged(EState PrevState, EState NewState)
{
	if (PrevState == EState::EDisabled || NewState == EState::EDisabled)
	{
		Update();
	}
}

void UXYXUpdateMeleeAIBehavior::HandleOnStaminaValueChanged(float NewValue, float MaxValue)
{
	if (NewValue <= 0.f)
	{
		bIsOutOfStamina = true;
		
		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([=]
			{
				bIsOutOfStamina = false;
			}
		);

		UWorld* World = GetWorld();
		check(World);
		FTimerHandle TmpTimer;
		World->GetTimerManager().SetTimer(TmpTimer, TimerCallback, 2.f, false);
	}
}
