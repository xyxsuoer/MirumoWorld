// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXDecoratorInRangeToTarget.h"
#include <Actors/XYXBaseNPC.h>
#include "Actors/XYXBaseAIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UXYXDecoratorInRangeToTarget::UXYXDecoratorInRangeToTarget()
{

}

bool UXYXDecoratorInRangeToTarget::PerformConditionCheckAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	auto ControlledCharacter = Cast<AXYXBaseNPC>(ControlledPawn);
	auto AIController = Cast<AXYXBaseAIController>(OwnerController);
	if (!AIController || !ControlledCharacter)
	{
		return false;
	}

	auto OtherActor = Cast<AActor>(AIController->BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
	float TmpValue = ControlledCharacter->GetDistanceTo(OtherActor);

	return UKismetMathLibrary::InRange_FloatFloat(TmpValue, GreaterThan, LessThan, true, true);
}
