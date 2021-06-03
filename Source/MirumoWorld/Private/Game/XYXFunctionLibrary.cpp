// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/XYXFunctionLibrary.h"
#include "Components/XYXExtendedStatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Game/XYXGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Game/XYXData.h"

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

float UXYXFunctionLibrary::GetCrosshairDistanceLocation()
{
	return 500.f;
}

UXYXExtendedStatComponent* UXYXFunctionLibrary::GetExtendedStatComp(AActor* Actor, EAttributesType Type)
{
	UXYXExtendedStatComponent* ExtendedStatComp = nullptr;

	auto StatsComps = Actor->K2_GetComponentsByClass(UXYXExtendedStatComponent::StaticClass());
	for (auto && e : StatsComps)
	{
		auto Comp = Cast<UXYXExtendedStatComponent>(e);
		if (Comp)
		{
			if (Comp->AttributesType == Type)
			{
				ExtendedStatComp = Comp;
			}
		}
	}

	return ExtendedStatComp;
}

float UXYXFunctionLibrary::ScaleMeleeDamageByType(float Damage, EMeleeAttackType MeleeAttackType)
{
	float TmpValue = 1.f;
	switch(MeleeAttackType)
	{
	case EMeleeAttackType::ENone:
		TmpValue = 1.0f;
		break;
	case EMeleeAttackType::ELight:
		TmpValue = 1.0f;
		break;
	case EMeleeAttackType::EHeavy:
		TmpValue = 1.5f;
		break;
	case EMeleeAttackType::ESpecial:
		TmpValue = 1.5f;
		break;
	case EMeleeAttackType::EThrust:
		TmpValue = 1.5f;
		break;
	case EMeleeAttackType::EFalling:
		TmpValue = 1.0f;
		break;
	}

	return Damage * TmpValue;
}

float UXYXFunctionLibrary::ScaleMeleeAttackStaminaCostByType(float Cost, EMeleeAttackType MeleeAttackType)
{
	float TmpValue = 1.f;
	switch (MeleeAttackType)
	{
	case EMeleeAttackType::ENone:
		TmpValue = 1.0f;
		break;
	case EMeleeAttackType::ELight:
		TmpValue = 1.0f;
		break;
	case EMeleeAttackType::EHeavy:
		TmpValue = 1.75f;
		break;
	case EMeleeAttackType::ESpecial:
		TmpValue = 1.75f;
		break;
	case EMeleeAttackType::EThrust:
		TmpValue = 1.75f;
		break;
	case EMeleeAttackType::EFalling:
		TmpValue = 0.75f;
		break;
	}

	return Cost * TmpValue;
}

EDirection UXYXFunctionLibrary::GetHitDirection(FVector HitFromDirection, AActor* AttackedActor)
{
	EDirection TmpDirection = EDirection::EFront;
	if (!AttackedActor)
	{
		return TmpDirection;
	}

	FVector AttackedActorForwardVector = AttackedActor->GetActorForwardVector();
	FVector TmpVectorOne = FVector(AttackedActorForwardVector.X, AttackedActorForwardVector.Y, 0.f);
	FVector TmpVectorTwo = FVector(HitFromDirection.X, HitFromDirection.Y, 0.f);
	float TmpDotProduct = UKismetMathLibrary::Dot_VectorVector(
		UKismetMathLibrary::Normal(TmpVectorOne, 0.0001), UKismetMathLibrary::Normal(TmpVectorTwo, 0.0001));

	if (TmpDotProduct >= 0.25f)
	{
		// Is In front
		TmpDirection = EDirection::EBack;
		return TmpDirection;
	}
	else if(TmpDotProduct <= -0.25)
	{
		// Is on back
		TmpDirection = EDirection::EFront;
		return TmpDirection;
	}

	FVector AttackedActorRightVector = AttackedActor->GetActorRightVector();
	TmpVectorOne = FVector(AttackedActorRightVector.X, AttackedActorRightVector.Y, 0.f);
	TmpVectorTwo = FVector(HitFromDirection.X, HitFromDirection.Y, 0.f);
	TmpDotProduct = UKismetMathLibrary::Dot_VectorVector(
		UKismetMathLibrary::Normal(TmpVectorOne, 0.0001), UKismetMathLibrary::Normal(TmpVectorTwo, 0.0001));

	if (TmpDotProduct >= 0.f)
	{
		TmpDirection = EDirection::ERight;
	}
	else
	{
		TmpDirection = EDirection::ELeft;
	}

	return TmpDirection;

}

void UXYXFunctionLibrary::PlayHitSound(UXYXGameInstance* GameInstance, AActor* ActorWhoHit, AActor* ActorWhoWasHit, FVector Location)
{
	if (!ActorWhoHit || !ActorWhoWasHit || !GameInstance)
	{
		return;
	}

	auto ApplierEquipment = Cast<UXYXEquipmentManagerComponent>(ActorWhoHit->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));
	auto ReceiverEquipment = Cast<UXYXEquipmentManagerComponent>(ActorWhoWasHit->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));
	if (ApplierEquipment && ReceiverEquipment)
	{
		USoundBase* TmpSound = nullptr; 
		switch(ApplierEquipment->GetWeaponType())
		{
		case EWeaponType::EBow: 
			{
				FSoundFX * pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("HitArrow"), "");
				if (pRow)
				{
					TmpSound = pRow->SoundBase;
				}
			}
			break;
		case EWeaponType::ENone:
			{
				FSoundFX* pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("HitHands"), "");
				if (pRow)
				{
					TmpSound = pRow->SoundBase;
				}
			}
			break;
		case EWeaponType::ESpell:
			{
				TmpSound = nullptr;
			}
			break;
		default:
			{
				FSoundFX* pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("HitSword"), "");
				if (pRow)
				{
					TmpSound = pRow->SoundBase;
				}
			}
			break;
		}

		if (TmpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(ActorWhoHit, TmpSound, Location);
		}
	}
}

void UXYXFunctionLibrary::PlayParrySound(UXYXGameInstance* GameInstance, AActor* ActorWhoParried, AActor* ActorWhoWasParried, FVector Location)
{
	if (!ActorWhoParried || !ActorWhoWasParried || !GameInstance)
	{
		return;
	}

	auto ApplierEquipment = Cast<UXYXEquipmentManagerComponent>(ActorWhoParried->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));
	auto ReceiverEquipment = Cast<UXYXEquipmentManagerComponent>(ActorWhoWasParried->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));
	if (ApplierEquipment && ReceiverEquipment)
	{
		USoundBase* TmpSound = nullptr; 

		if (ApplierEquipment->IsShieldEquipped())
		{
			FSoundFX* pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("BlockShieldMetal"), "");
			if (pRow)
			{
				TmpSound = pRow->SoundBase;
			}
		}
		else
		{
				if (ReceiverEquipment->IsWeaponEquipped())
				{
					switch (ReceiverEquipment->GetWeaponType())
					{
					case EWeaponType::ENone:
					case EWeaponType::ESpell:
					{
						TmpSound = nullptr;
					}
					break;
					default:
					{
						FSoundFX* pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("BlockMetalMetal"), "");
						if (pRow)
						{
							TmpSound = pRow->SoundBase;
						}
					}
					break;
					}
				}
		}
		
		if (TmpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(ActorWhoWasParried, TmpSound, Location);
		}
	}
}

void UXYXFunctionLibrary::PlayBlockSound(UXYXGameInstance* GameInstance, AActor* ActorWhoBlocked, AActor* ActorWhoWasBlocked, FVector Location)
{
	if (!ActorWhoBlocked || !ActorWhoWasBlocked || !GameInstance)
	{
		return;
	}

	auto ApplierEquipment = Cast<UXYXEquipmentManagerComponent>(ActorWhoBlocked->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));
	auto ReceiverEquipment = Cast<UXYXEquipmentManagerComponent>(ActorWhoWasBlocked->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));
	if (ApplierEquipment && ReceiverEquipment)
	{
		USoundBase* TmpSound = nullptr;

		if (ApplierEquipment->IsShieldEquipped())
		{
			if (ReceiverEquipment->GetWeaponType() == EWeaponType::ESpell)
			{
				TmpSound = nullptr;
			}
			else
			{
				FSoundFX* pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("BlockShieldMetal"), "");
				if (pRow)
				{
					TmpSound = pRow->SoundBase;
				}
			}
		}
		else
		{
			if (ApplierEquipment->IsWeaponEquipped())
			{
				switch (ApplierEquipment->GetWeaponType())
				{
				case EWeaponType::ENone:
				{
					TmpSound = nullptr;
				}
				break;
				default:
				{
					FSoundFX* pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("BlockMetalMetal"), "");
					if (pRow)
					{
						TmpSound = pRow->SoundBase;
					}
				}
				break;
				}
			}
			else
			{
				FSoundFX* pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("HitSword"), "");
				if (pRow)
				{
					TmpSound = pRow->SoundBase;
				}
			}
		}

		if (TmpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(ActorWhoWasBlocked, TmpSound, Location);
		}
	}
}

AActor* UXYXFunctionLibrary::GetClosestActor(AActor* Target, TArray<AActor*> Actors)
{
	AActor* TmpClosestActor = nullptr;
	if (!Target || Actors.Num() == 0)
	{
		return TmpClosestActor;
	}

	TmpClosestActor = Actors[0];
	float TmpClosestDistance = 0;
	for (int32 i =0; i < Actors.Num(); ++i) 
	{
		if (i == 0)
		{
			TmpClosestActor = Actors[0];
			TmpClosestDistance = Target->GetSquaredDistanceTo(Actors[0]);
		}
		else
		{
			if (Target->GetSquaredDistanceTo(Actors[i]) < TmpClosestDistance)
			{
				TmpClosestDistance = Target->GetSquaredDistanceTo(Actors[i]);
				TmpClosestActor = Actors[i];
			}
		}
	}

	return TmpClosestActor;
}
