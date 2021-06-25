// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXUpdateArcherAIBehavior.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include <Actors/XYXBaseNPC.h>
#include "BehaviorTree/BTFunctionLibrary.h"
#include <Actors/XYXBaseAIController.h>
#include "Components/XYXStateManagerComponent.h"
#include "Components/XYXExtendedStatComponent.h"
#include "Components/XYXStatsManagerComponent.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/XYXData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Components/XYXPatrolComponent.h"

UXYXUpdateArcherAIBehavior::UXYXUpdateArcherAIBehavior()
{
	bCreateNodeInstance = true;
}

void UXYXUpdateArcherAIBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Update();
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

void UXYXUpdateArcherAIBehavior::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	AIController = Cast<AXYXBaseAIController>(SearchData.OwnerComp.GetAIOwner());
	if (AIController)
		ControlledCharacter = Cast<AXYXBaseNPC>(AIController->PossesedAI);
	if (ControlledCharacter)
	{
		UXYXStateManagerComponent* StateComp = ControlledCharacter->GetStateManagerComponent();
		if (StateComp)
		{
			StateComp->OnStateChanged.AddUniqueDynamic(this, &UXYXUpdateArcherAIBehavior::HandleOnStateChanged);
		}
	}

	Super::OnSearchStart(SearchData);
}

void UXYXUpdateArcherAIBehavior::Update()
{
	UpdateBehavior();
	UpdateActivities();
}

void UXYXUpdateArcherAIBehavior::UpdateBehavior()
{
	if (!AIController || !ControlledCharacter || !AIController->GetBlackboardComponent())
	{
		return;
	}

	auto StatsComp = ControlledCharacter->GetStatsManagerComponent();
	auto StateComp = ControlledCharacter->GetStateManagerComponent();
	auto ExtendedStamina = ControlledCharacter->GetExtendedStamina();
	if (!StatsComp || !StateComp || !ExtendedStamina)
	{
		return;
	}

	// Check if controlled character's state is not disabled
	if (StateComp->GetState() == EState::EDisabled)
	{
		SetBehavior(EAIBehavior::EHit);
	}
	else
	{
		auto TmpTarget = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
		if (TmpTarget)
		{
			IXYXInterfaceEntity* Entity = Cast<IXYXInterfaceEntity>(TmpTarget);
			// Check if target is set and if it's alive
			if (Entity && Entity->Execute_IsEntityAlive(TmpTarget))
			{
				float TmpDistanceToTarget = TmpTarget->GetDistanceTo(ControlledCharacter);
				float TmpDotProductToTarget = TmpTarget->GetDotProductTo(ControlledCharacter);
				int32 TmpReceivedHitCount = StatsComp->GetRecentlyReceivedHitsCount();
				float TmpStaminaPercent = ExtendedStamina->GetCurrentValue() / ExtendedStamina->GetMaxValue() * 100.f;
				auto TargetEquipment = Cast<UXYXEquipmentManagerComponent>(TmpTarget->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));

				if (bIsFleeing)
				{
					SetBehavior(EAIBehavior::EFlee);
					Flee();
				}
				else
				{
					if (TmpDistanceToTarget <= FleeBehaviorRange)
					{
						if (TargetEquipment)
						{
							// Does target use Magic or Range combat type
							if (TargetEquipment->GetCombatType() == ECombatType::EMagic ||
								TargetEquipment->GetCombatType() == ECombatType::ERanged)
							{
								SetBehavior(EAIBehavior::ERangeAttack);
							}
							else
							{
								SetBehavior(EAIBehavior::EFlee);
								Flee();
							}
						}
						else
						{
							SetBehavior(EAIBehavior::EFlee);
							Flee();
						}
					}
					else
					{
						if (TmpDistanceToTarget >= 2500.f)
						{
							SetBehavior(EAIBehavior::EApproach);
						}
						else
						{
							SetBehavior(EAIBehavior::ERangeAttack);
						}
					}
				}
			}
			else
			{
				if (ControlledCharacter->PatrolComp)
				{
					if (ControlledCharacter->PatrolComp->IsPatrolPathValid())
					{
						SetBehavior(EAIBehavior::EPatrol);
					}
					else
					{
						SetBehavior(EAIBehavior::EIdle);
					}
				}
			}
		}
	}
}

void UXYXUpdateArcherAIBehavior::UpdateActivities()
{
	if (!AIController || !ControlledCharacter || !AIController->GetBlackboardComponent())
	{
		return;
	}

	auto  StateComp = ControlledCharacter->GetStateManagerComponent();
	if (!StateComp)
	{
		return;
	}

	// Turn On/Off blocking
	auto TmpSelection = (EAIBehavior)AIController->GetBlackboardComponent()->GetValueAsEnum(BehaviorKey.SelectedKeyName);

	switch (TmpSelection)
	{
	case EAIBehavior::EIdle:
	case EAIBehavior::EPatrol:
	case EAIBehavior::EApproach:
	case EAIBehavior::EStrafeAround:
	case EAIBehavior::EFlee:
	case EAIBehavior::EHit:
	{
		StateComp->SetActivity(EActivity::EIsAmingPressed, false);
	}
	break;
	case EAIBehavior::ERangeAttack:
	{
		bool TmpValue = StateComp->GetState() == EState::EIdle && ControlledCharacter &&
			ControlledCharacter->GetEquipmentManagerComponent() &&
			ControlledCharacter->GetEquipmentManagerComponent()->GetIsInCombat();

		StateComp->SetActivity(EActivity::EIsAmingPressed, TmpValue);

	}
	break;
	}
}

void UXYXUpdateArcherAIBehavior::SetBehavior(EAIBehavior Behavior)
{
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(BehaviorKey.SelectedKeyName, (uint8)Behavior);
	}
}

void UXYXUpdateArcherAIBehavior::HandleOnStateChanged(EState PrevState, EState NewState)
{
	if (PrevState == EState::EDisabled || NewState == EState::EDisabled || NewState == EState::EAttacking)
	{
		Update();
	}
}

void UXYXUpdateArcherAIBehavior::Flee()
{
	if (AIController)
	{
		AIController->UpdateSenseTarget();
		bIsFleeing = true;

		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([=]
			{
				bIsFleeing = false;
			}
		);

		UWorld* World = GetWorld();
		check(World);
		FTimerHandle TmpTimer;
		World->GetTimerManager().SetTimer(TmpTimer, TimerCallback, 3.f, false);
	}
}
