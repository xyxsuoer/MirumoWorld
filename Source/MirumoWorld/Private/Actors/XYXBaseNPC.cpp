// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/XYXBaseNPC.h"
#include <Components/XYXMontageManagerComponent.h>
#include <Components/XYXStateManagerComponent.h>
#include <Components/XYXMovementSpeedComponent.h>
#include <Components/XYXEquipmentManagerComponent.h>
#include <Components/XYXRotatingComponent.h>
#include <Components/XYXCollisionHandlerComponent.h>
#include <Components/XYXEffectsComponent.h>
#include <Animations/XYXAnimInstance.h>
#include <Net/UnrealNetwork.h>

// Sets default values
AXYXBaseNPC::AXYXBaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MontageManagerComp = CreateDefaultSubobject<UXYXMontageManagerComponent>(TEXT("Montage Manager Component"));
	StateManagerComp = CreateDefaultSubobject<UXYXStateManagerComponent>(TEXT("State Manager Component"));
	MovementSpeedComp = CreateDefaultSubobject<UXYXMovementSpeedComponent>(TEXT("Movement Speed Component"));
	EquipmentComp = CreateDefaultSubobject<UXYXEquipmentManagerComponent>(TEXT("Equipment Manager Component"));
	RotatingComp = CreateDefaultSubobject<UXYXRotatingComponent>(TEXT("Rotating Component"));
	CollisionHandlerComp = CreateDefaultSubobject<UXYXCollisionHandlerComponent>(TEXT("Collision Handler Component"));
	EffectsManagerComp = CreateDefaultSubobject<UXYXEffectsComponent>(TEXT("Effects Manager Component"));
}

// Called when the game starts or when spawned
void AXYXBaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

UXYXAnimInstance* AXYXBaseNPC::GetXYXAnimInstance() const {
	return Cast<UXYXAnimInstance>(GetMesh()->GetAnimInstance());
}

void AXYXBaseNPC::SetGenericTeamId(const FGenericTeamId& InTeamID) {
	CombatTeam = (ETeam)InTeamID.GetId();
}

FGenericTeamId AXYXBaseNPC::GetGenericTeamId() const {
	return uint8(CombatTeam);
}

void AXYXBaseNPC::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AXYXBaseNPC, CombatTeam);
}

UDataTable* AXYXBaseNPC::GetMontages_Implementation(EMontageAction Action)
{
	return nullptr;
}

bool AXYXBaseNPC::IsEntityAlive_Implementation()
{
	if (StateManagerComp)
	{
		return StateManagerComp->GetState() == EState::EDead ? false : true;
	}

	return true;
}

FName AXYXBaseNPC::GetHeadSocket_Implementation()
{
	return TEXT("head");
}

FRotator AXYXBaseNPC::GetDesiredRotation_Implementation()
{
	return FRotator::ZeroRotator;
}

bool AXYXBaseNPC::TakeAttackDamage_Implementation(FHitData HitData, EAttackResult& ResultType)
{
	return false;
}

bool AXYXBaseNPC::CanEffectBeApplied_Implementation(EEffectType Type, AActor* Applier)
{
	if (CanBeAttacked())
	{
		switch(Type)
		{
		case EEffectType::EStun:
			{
				return CanBeStunned() && CanBeInterrupted();
			}
			break;
		case EEffectType::EBurning:
			{
				return true;
			}
		break;
		case EEffectType::EBackstab:
			{
				return CanBeBackstabbed() && CanBeInterrupted();
			}
		break;
		case EEffectType::EImpact:
			{
				return CanBeInterrupted();
			}
		break;
		case EEffectType::EParried:
			{
				return CanBeInterrupted();
			}
			break;
		}
	}

	return false;
}

bool AXYXBaseNPC::OnSelected_Implementation()
{
	return false;
}

bool AXYXBaseNPC::OnDeselected_Implementation()
{
	return false;
}

bool AXYXBaseNPC::IsTargetable_Implementation()
{
	return IsEntityAlive();
}

UAnimMontage* AXYXBaseNPC::GetRollMontage(EDirection Direction)
{
	UAnimMontage* Montage = nullptr;
	EMontageAction RollDirection = EMontageAction::EStunFront;

	switch (Direction)
	{
	case EDirection::EFront:
		RollDirection = EMontageAction::ERollForward;
		break;
	case EDirection::EBack:
		RollDirection = EMontageAction::ERollBackward;
		break;
	case EDirection::ELeft:
		RollDirection = EMontageAction::ERollLeft;
		break;
	case EDirection::ERight:
		RollDirection = EMontageAction::ERollRight;
		break;
	}

	if (MontageManagerComp)
		Montage = MontageManagerComp->GetMontageForAction(RollDirection, 0);

	return Montage;
}

UAnimMontage* AXYXBaseNPC::GetBlockMontage()
{
	UAnimMontage* Montage = nullptr;

	if (EquipmentComp && MontageManagerComp)
	{
		int32 Index = EquipmentComp->IsShieldEquipped() ? 1 : 0;
		Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EBlock, Index);
	}

	return Montage;
}

UAnimMontage* AXYXBaseNPC::GetImpactMontage()
{
	UAnimMontage* Montage = nullptr;

	if (MontageManagerComp) 
	{
		int32 TmpIndex = MontageManagerComp->GetRandomMontageIndex(EMontageAction::EImpact);
		Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EImpact, TmpIndex);
	}

	return Montage;
}

UAnimMontage* AXYXBaseNPC::GetParriedMontage()
{
	UAnimMontage* Montage = nullptr;

	if (MontageManagerComp)
	{
		int32 TmpIndex = MontageManagerComp->GetRandomMontageIndex(EMontageAction::EParried);
		Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EParried, TmpIndex);
	}

	return Montage;
}

bool AXYXBaseNPC::CanBeStunned()
{
	if (EffectsManagerComp && EffectsManagerComp->IsEffectApplied(EEffectType::EBackstab))
	{
		return false;
	}
	else
	{
		// allow to be stunned if received damage is less than limit
		// but if received damage exceeds limit, check if hit count is less or eq to limit
	}
	return true;
}

bool AXYXBaseNPC::CanBeAttacked()
{
	if (IsEntityAlive() && StateManagerComp && !StateManagerComp->GetActivityValue(EActivity::EIsImmortal))
	{
		return true;
	}

	return false;
}

bool AXYXBaseNPC::CanBeBackstabbed()
{
	return false;
}

void AXYXBaseNPC::ShowStatsWidget()
{

}

void AXYXBaseNPC::HideStatsWidget()
{

}

void AXYXBaseNPC::Death()
{

}

void AXYXBaseNPC::Stunned()
{

}

void AXYXBaseNPC::Block()
{

}

void AXYXBaseNPC::Parried()
{

}

void AXYXBaseNPC::Impact()
{

}

void AXYXBaseNPC::Backstabbed()
{

}

FHitData AXYXBaseNPC::MakeMeleeHitData(AActor* HitActor)
{
	FHitData TmpData; 
	return TmpData;
}

void AXYXBaseNPC::ApplyHitImpulseToCharacter(AActor* HitActor, FVector HitNormal, float ImpulsePower)
{

}

float AXYXBaseNPC::MeleeAttack(EMeleeAttackType Type)
{
	return 0.f;
}

UAnimMontage* AXYXBaseNPC::GetMeleeAttackMontage(EMeleeAttackType Type)
{
	return nullptr;
}

void AXYXBaseNPC::ResetMeleeAttackCounter()
{

}

float AXYXBaseNPC::Roll(EDirection Direction)
{
	return 0.f;
}

void AXYXBaseNPC::OnCollisionActivated(ECollisionPart Selection)
{

}

bool AXYXBaseNPC::CanBeInterrupted()
{
	if (StateManagerComp )
	{
		return  !StateManagerComp->GetActivityValue(EActivity::ECanBeInterrupted);
	}

	return true;
}

