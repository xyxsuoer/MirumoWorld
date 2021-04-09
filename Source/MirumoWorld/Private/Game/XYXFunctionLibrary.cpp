// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/XYXFunctionLibrary.h"

EMontageAction UXYXFunctionLibrary::ConvertMeleeAttackTypeToAction(const EMeleeAttackType Type)
{
	EMontageAction Action = EMontageAction::ENone; 
	switch(Type)
	{
	case EMeleeAttackType::ENone:
		Action = EMontageAction::ENone;
		break;
	case EMeleeAttackType::ELight:
		Action = EMontageAction::ELightAttack;
		break;
	case EMeleeAttackType::EHeavy:
		Action = EMontageAction::EHeavyAttack;
		break;
	case EMeleeAttackType::EFalling:
		Action = EMontageAction::EFallingAttack;
		break;
	case EMeleeAttackType::ESpecial:
		Action = EMontageAction::ESpecialAttack;
		break;
	case EMeleeAttackType::EThrust:
		Action = EMontageAction::EThrustAttack;
		break;
	}

	return Action;
}
