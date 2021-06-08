// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/XYXBaseAIController.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Actors/XYXBaseNPC.h"
#include "Interfaces/XYXInterfaceEntity.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/XYXFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include <Components/XYXEquipmentManagerComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "BehaviorTree/BlackboardData.h"
#include <BehaviorTree/BehaviorTree.h>


AXYXBaseAIController::AXYXBaseAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	if (AIPerceptionComp)
	{
		UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("UAISenseConfig_Sight"));
		check(SightConfig);
		SightConfig->SightRadius = 2000;
		SightConfig->LoseSightRadius = 2500;
		SightConfig->PeripheralVisionAngleDegrees = 65;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = FAISystem::InvalidRange;
		SightConfig->SetMaxAge(5.f);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->PointOfViewBackwardOffset = 0.f;
		SightConfig->NearClippingRadius = 0.f;
		AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
		AIPerceptionComp->ConfigureSense(*SightConfig);

		UAISenseConfig_Damage* DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("UAISenseConfig_Damage"));
		check(DamageConfig);
		DamageConfig->SetMaxAge(10.0f);
		AIPerceptionComp->ConfigureSense(*DamageConfig);
	}
}

void AXYXBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossesedAI = Cast<AXYXBaseNPC>(InPawn);
	if (PossesedAI->BehaviorTree)
	{
		UBlackboardData*  bbData = PossesedAI->BehaviorTree->BlackboardAsset;
		if (bbData == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("This behavior Tree should be assigned with a blackborad, %s"), *InPawn->GetName());
			return;
		}

		BlackboardComponent->InitializeBlackboard(*bbData);
		Blackboard = BlackboardComponent;

		RunBehaviorTree(PossesedAI->BehaviorTree);
		
		UWorld* World = GetWorld();
		check(World);

		World->GetTimerManager().SetTimer(UpdateTargetTimer, this, &AXYXBaseAIController::UpdateTarget, 1.f, true);

		if (PossesedAI->GetEquipmentManagerComponent())
		{
			PossesedAI->GetEquipmentManagerComponent()->OnInCombatChanged.AddDynamic(this, &AXYXBaseAIController::HandleOnInCombatChanged);
		}
		
	}
}

void AXYXBaseAIController::HandleOnInCombatChanged(bool bValue)
{
	SetIsInCombat(bValue);
}

void AXYXBaseAIController::UpdateTarget()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	auto ControlledPawn = this->GetPawn();

	IXYXInterfaceEntity* ControlledEntity = Cast<IXYXInterfaceEntity>(ControlledPawn);
	if (!ControlledEntity || !ControlledEntity->Execute_IsEntityAlive(ControlledPawn) || !AIPerceptionComp)
	{
		return;
	}

	TArray<AActor*> KnownActors;
	TArray<AActor*> TmpPerceivedActors;
	TArray<AActor*> TmpEnemyActors;
	PerceptionComponent->GetKnownPerceivedActors(UAISense::StaticClass(), KnownActors);
	for (auto&& e : KnownActors)
	{
		IXYXInterfaceEntity* Entity = Cast<IXYXInterfaceEntity>(e);
		if (Entity && Entity->Execute_IsEntityAlive(e))
		{
			TmpPerceivedActors.Emplace(e);
		}
	}

	for (auto&& e: TmpPerceivedActors)
	{
		IXYXInterfaceEntity* Entity = Cast<IXYXInterfaceEntity>(e);
		if (Entity && Entity->Execute_GetEntityCombatTeam(e) == ETeam::EHeroesTeam)
		{
			TmpEnemyActors.Emplace(e);
		}
	}

	if (TmpEnemyActors.Num() > 0)
	{
		AActor* ClosestActor = UXYXFunctionLibrary::GetClosestActor(ControlledPawn, TmpEnemyActors);
		SetTarget(ClosestActor);
	}
	else
	{
		SetTarget(nullptr);
	}

	// If player is in perceived actors, show health bar
	if (PlayerController && TmpPerceivedActors.Contains(PlayerController))
	{
		PossesedAI->ShowStatsWidget();
	}
	else
	{
		PossesedAI->HideStatsWidget();
	}
}

void AXYXBaseAIController::SetTarget(AActor* NewTarget)
{
	if (NewTarget != Target)
	{
		Target = NewTarget;

		if (Blackboard)
		{
			Blackboard->SetValueAsObject(TargetKey, NewTarget);
		}
	}
}

void AXYXBaseAIController::SetIsInCombat(bool bValue)
{
	if (bIsInCombat != bValue)
	{
		bIsInCombat = bValue;
		if (Blackboard)
		{
			Blackboard->SetValueAsBool(IsInCombatKey, bIsInCombat);
		}
	}
}

void AXYXBaseAIController::UpdateSenseTarget()
{
	if (AIPerceptionComp)
	{
		auto ControlledPawn = this->GetPawn();
		auto World = GetWorld();
		check(World);
		UAISense_Damage::ReportDamageEvent(World, ControlledPawn, Target, 1.f, ControlledPawn->GetActorLocation(), ControlledPawn->GetActorLocation());
	}
}
