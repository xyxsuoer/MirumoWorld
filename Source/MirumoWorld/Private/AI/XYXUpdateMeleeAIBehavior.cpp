// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXUpdateMeleeAIBehavior.h"
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

UXYXUpdateMeleeAIBehavior::UXYXUpdateMeleeAIBehavior()
{
	
}

void UXYXUpdateMeleeAIBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	Update();
}

void UXYXUpdateMeleeAIBehavior::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	AIController = Cast<AXYXBaseAIController>(SearchData.OwnerComp.GetAIOwner());
	if(AIController)
		ControlledCharacter = Cast<AXYXBaseNPC>(AIController->PossesedAI);
	if (ControlledCharacter)
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

	auto StatsComp = ControlledCharacter->GetStatsManagerComponent();
	auto StateComp = ControlledCharacter->GetStateManagerComponent();
	auto ExtendedStamina = ControlledCharacter->GetExtendedStamina();
	if (!StatsComp || !ExtendedStamina)
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
		auto TmpTarget =  UBTFunctionLibrary::GetBlackboardValueAsActor(this, TargetKey);
		IXYXInterfaceEntity* Entity = Cast<IXYXInterfaceEntity>(TmpTarget);
		// Check if target is set and if it's alive
		if (Entity && Entity->Execute_IsEntityAlive(TmpTarget))
		{
			float TmpDistanceToTarget = TmpTarget->GetDistanceTo(ControlledCharacter);
			float TmpDotProductToTarget = TmpTarget->GetDotProductTo(ControlledCharacter);
			int32 TmpReceivedHitCount = StatsComp->GetRecentlyReceivedHitsCount();
			float TmpStaminaPercent = ExtendedStamina->GetCurrentValue() / ExtendedStamina->GetMaxValue() * 100.f;
			auto TargetEquipment = Cast<UXYXEquipmentManagerComponent>(TmpTarget->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));

			// Is Behind target
			if (TmpDotProductToTarget <= -0.25f)
			{
				// Is in attack range
				if (TmpDistanceToTarget <= AttackBehaviorRange)
				{
					SetBehavior(EAIBehavior::EMeleeAttack);
				}
				else
				{
					SetBehavior(EAIBehavior::EApproach);
				}
			}
			else
			{
				// Is in attack range
				if (TmpDistanceToTarget <= AttackBehaviorRange)
				{
					// If is close to target for at least 3 ticks
					if (TicksNearTarget >= 3.0f)
					{
						SetBehavior(EAIBehavior::EMeleeAttack);
						// Reset Ticks Near Target with some chance to avoid non stop attacking
						if (UKismetMathLibrary::RandomBoolWithWeight(0.4f))
						{
							TicksNearTarget = 0.f;
						}
					}
					else
					{
						TicksNearTarget += 1.f;
						// Whether AI should attack or strafe around blocking
						if (bIsOutOfStamina || TmpStaminaPercent <= 40.f || UKismetMathLibrary::RandomBoolWithWeight(0.1f))
						{
							SetBehavior(EAIBehavior::EMeleeAttack);
						}
						else
						{
							if (TargetEquipment)
							{
								// Does target use Magic or Range combat type
								if (TargetEquipment->GetCombatType() == ECombatType::EMagic || 
									TargetEquipment->GetCombatType() == ECombatType::ERanged)
								{
									SetBehavior(EAIBehavior::EMeleeAttack);
								}
								else
								{
									SetBehavior(EAIBehavior::EStrafeAround);
								}
							}
							else
							{
								SetBehavior(EAIBehavior::EStrafeAround);
							}
						}
					}
				}
				else
				{
					TicksNearTarget = 0.f;
					if (TmpDistanceToTarget >= 2000.f)
					{
						SetBehavior(EAIBehavior::EApproach);
					}
					else
					{
						if (TargetEquipment)
						{
							// Is target in Combat
							if (TargetEquipment->GetIsInCombat())
							{
								// Does target use Magic or Range combat type
								if (TargetEquipment->GetCombatType() == ECombatType::EMagic || 
									TargetEquipment->GetCombatType() == ECombatType::ERanged)
								{
									SetBehavior(EAIBehavior::EApproach);
								}
								else
								{
									SetBehavior(EAIBehavior::EStrafeAround);
								}
							}
							else
							{
								SetBehavior(EAIBehavior::EApproach);
							}
						}
						else
						{
							SetBehavior(EAIBehavior::EApproach);
						}
					}
				}
			}
		}
		else
		{
			// TODO Patrol
			SetBehavior(EAIBehavior::EIdle);
		}
	}

}

void UXYXUpdateMeleeAIBehavior::UpdateActivities()
{
	if (!AIController || !ControlledCharacter)
	{
		return;
	}

	auto  StateComp = ControlledCharacter->GetStateManagerComponent();
	if (!StateComp)
	{
		return;
	}

	// Turn On/Off blocking
	auto TmpSelection = (EAIBehavior)UBTFunctionLibrary::GetBlackboardValueAsEnum(this, BehaviorKey);
	switch(TmpSelection)
	{
		case EAIBehavior::EIdle:
		case EAIBehavior::EPatrol:
		case EAIBehavior::EMeleeAttack:
		case EAIBehavior::EFlee:
		case EAIBehavior::EHit:
			{
				StateComp->SetActivity(EActivity::EIsBlockingPressed, false);
			}
			break;
		case EAIBehavior::EApproach:
		case EAIBehavior::EStrafeAround:
			{
				bool TmpValue = !bIsOutOfStamina && ControlledCharacter &&
					ControlledCharacter->GetEquipmentManagerComponent() &&
					ControlledCharacter->GetEquipmentManagerComponent()->GetIsInCombat();
				
				StateComp->SetActivity(EActivity::EIsBlockingPressed, TmpValue);

			}
			break;
	}	
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
