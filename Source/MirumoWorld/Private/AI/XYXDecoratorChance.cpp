// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/XYXDecoratorChance.h"
#include "Kismet/KismetMathLibrary.h"

UXYXDecoratorChance::UXYXDecoratorChance()
{

}

bool UXYXDecoratorChance::PerformConditionCheckAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	return UKismetMathLibrary::RandomIntegerInRange(1, 100) <= Chance;
}
