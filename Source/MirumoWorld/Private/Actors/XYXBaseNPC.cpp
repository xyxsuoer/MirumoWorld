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
#include "Components/XYXStatsManagerComponent.h"
#include "Components/XYXExtendedStatComponent.h"
#include <Net/UnrealNetwork.h>
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <Actors/XYXCharacter.h>
#include "Actors/XYXBaseAIController.h"
#include "Game/XYXGameInstance.h"
#include "Game/XYXFunctionLibrary.h"
#include "Perception/AISense_Damage.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Items/DisplayedItems/XYXDisplayedItem.h"
#include "UI/XYXUserWidgetAIStatBar.h"

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
	StatsManagerComp = CreateDefaultSubobject<UXYXStatsManagerComponent>(TEXT("Stats Manager Component"));
	ExtendedHealth = CreateDefaultSubobject<UXYXExtendedStatComponent>(TEXT("Extended Stat Health"));
	ExtendedStamina = CreateDefaultSubobject<UXYXExtendedStatComponent>(TEXT("Extended Stat Stamina"));

	TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Target Widget"));
	TargetWidget->SetupAttachment(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Type::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AXYXBaseAIController::StaticClass();

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

}

// Called when the game starts or when spawned
void AXYXBaseNPC::BeginPlay()
{
	Super::BeginPlay();

	if (StatsManagerComp)
	{
		StatsManagerComp->Initialize();
	}

	if (EquipmentComp)
	{
		EquipmentComp->Initialize();
	}

	if (CollisionHandlerComp)
	{
		CollisionHandlerComp->OnCollisionActivated.AddDynamic(this, &AXYXBaseNPC::HandleOnCollisionActivated);
	}

	if (ExtendedHealth)
	{
		ExtendedHealth->SetCurrentValue(ExtendedHealth->GetMaxValue(), false);
		ExtendedHealth->OnValueChanged.AddDynamic(this, &AXYXBaseNPC::HandleExtendedHealthOnValueChanged);
	}

	if (EffectsManagerComp)
	{
		EffectsManagerComp->OnEffectApplied.AddDynamic(this, &AXYXBaseNPC::HandleOnEffectApplied);
	}

	BaseAIController = Cast<AXYXBaseAIController>(GetController());

	StatsBarWidget = CreateWidget<UXYXUserWidgetAIStatBar>(GetWorld(), WBStatsBarClass);

	InitializeStatsWidget();
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
	FRotator TmpRotator = this->GetActorRotation();
	if (BaseAIController)
	{
		auto BlackboardComp = BaseAIController->FindComponentByClass<UBlackboardComponent>();
		if (BlackboardComp && BaseAIController->Target)
		{
			FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), BaseAIController->Target->GetActorLocation());
			TmpRotator.Yaw = LookAtRotator.Yaw;
		}
	}

	return TmpRotator;
}

bool AXYXBaseNPC::TakeAttackDamage_Implementation(FHitData HitData, EAttackResult& ResultType)
{
	UWorld* World = GetWorld();
	check(World);

	ResultType = EAttackResult::EFailed;
	bool TmpResult = false;
	if (CanBeAttacked())
	{
		UpdateReceivedHitDirection(HitData.HitFromDirection);
		if (HitData.Damage != 0.f)
		{
			// Report received damage to Damage Sense
			UAISense_Damage::ReportDamageEvent(World, this, HitData.DamageCauser, HitData.Damage, GetActorLocation(), GetActorLocation());

			// If hit was successfully Parried, apply Parried effect on attacker and don't subtract health
			bool bTmpParried = ReceivedHitDirection == EDirection::EFront &&
				StateManagerComp && StateManagerComp->GetActivityValue(EActivity::ECanParryHit) &&
				HitData.CanBeParried;

			if (bTmpParried)
			{
				auto EffectComp = Cast<UXYXEffectsComponent>(HitData.DamageCauser->GetComponentByClass(UXYXEffectsComponent::StaticClass()));
				if (EffectComp)
				{
					bool bTmpApplied = EffectComp->ApplyEffect(EEffectType::EParried, 1.f, EApplyEffectMethod::EReplace, this);
					if (bTmpApplied)
					{
						UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(World->GetGameInstance());
						UXYXFunctionLibrary::PlayParrySound(GameInstance, this, HitData.DamageCauser, GetActorLocation());
					}
				}
				ResultType = EAttackResult::EParried;
				return TmpResult;
			}

			// Check if hit was successfully blocked
			bool bTmpBlocked = ReceivedHitDirection == EDirection::EFront && StateManagerComp && 
				StateManagerComp->GetActivityValue(EActivity::EIsBlockingPressed) && HitData.CanBeBlocked;

			// Ignore stamina if hit was not blocked
			if (StatsManagerComp)
			{
				StatsManagerComp->TakeDamage(HitData.Damage, !bTmpBlocked);

				// If hit was blockedand character is still alive, call block function
				if (Execute_IsEntityAlive(this) && bTmpBlocked)
				{
					UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(World->GetGameInstance());
					UXYXFunctionLibrary::PlayBlockSound(GameInstance, this, HitData.DamageCauser, this->GetActorLocation());

					Block();

					// If there is still some stamina left after blocked hit, try to apply Impact effect on attacker
					if (ExtendedStamina && ExtendedStamina->GetCurrentValue() > 0.f)
					{
						// Don't apply if hit was blocked with hands
						if (EquipmentComp &&
							(EquipmentComp->IsShieldEquipped() || !(EquipmentComp->GetCombatType() == ECombatType::EUnarmed)))
						{
							auto EffectComp = Cast<UXYXEffectsComponent>(HitData.DamageCauser->GetComponentByClass(UXYXEffectsComponent::StaticClass()));
							if (EffectComp)
							{
								bool bTmpApplied = EffectComp->ApplyEffect(EEffectType::EImpact, 1.f, EApplyEffectMethod::EReplace, this);
							}
						}
					}

					ResultType = EAttackResult::EBlocked;
					return TmpResult;
				}
			}

			ResultType = EAttackResult::ESuccess;
			TmpResult = true;
		}
	}

	return TmpResult;
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
	if (TargetWidget)
	{
		TargetWidget->SetHiddenInGame(false, false);
	}
	return false;
}

bool AXYXBaseNPC::OnDeselected_Implementation()
{
	if (TargetWidget)
	{
		TargetWidget->SetHiddenInGame(true, false);
	}

	return false;
}

bool AXYXBaseNPC::IsTargetable_Implementation()
{
	return Execute_IsEntityAlive(this);
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

UAnimMontage* AXYXBaseNPC::GetStunMontage(EDirection Direction)
{
	UAnimMontage* Montage = nullptr;
	EMontageAction StunDirection = EMontageAction::EStunFront;

	switch (Direction)
	{
	case EDirection::EFront:
		StunDirection = EMontageAction::EStunFront;
		break;
	case EDirection::EBack:
		StunDirection = EMontageAction::EStunBack;
		break;
	case EDirection::ELeft:
		StunDirection = EMontageAction::EStunLeft;
		break;
	case EDirection::ERight:
		StunDirection = EMontageAction::EStunRight;
		break;
	}

	if (MontageManagerComp)
	{
		int32 TmpIndex = MontageManagerComp->GetRandomMontageIndex(StunDirection);
		Montage = MontageManagerComp->GetMontageForAction(StunDirection, TmpIndex);
	}

	if (!Montage)
	{
		if (StunDirection != EMontageAction::EStunFront)
		{
			int32 TmpIndex = MontageManagerComp->GetRandomMontageIndex(EMontageAction::EStunFront);
			Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EStunFront, TmpIndex);
		}
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
		if (StatsManagerComp)
		{
			if (StatsManagerComp->GetRecentlyReceivedSuccessfulDamage() < RecentlyReceivedDamageStunLimit)
			{
				return true;
			}
			else
			{
				// but if received damage exceeds limit, check if hit count is less or eq to limit
				return StatsManagerComp->GetRecentlyReceivedSuccessfulHitsCount() <= RecentlyReceivedHitsCountStunLimit;
			}
		}
		
	}
	return true;
}

bool AXYXBaseNPC::CanBeAttacked()
{
	if (Execute_IsEntityAlive(this))
	{
		if (StateManagerComp)
		{
			return !StateManagerComp->GetActivityValue(EActivity::EIsImmortal);
		}
	}

	return false;
}

bool AXYXBaseNPC::CanBeBackstabbed()
{
	if (EffectsManagerComp && !EffectsManagerComp->IsEffectApplied(EEffectType::EBackstab) && 
		BaseAIController && !BaseAIController->Target)
	{
		return true;
	}

	return false;
}

void AXYXBaseNPC::ShowStatsWidget()
{
	if (StatsBarWidget)
	{
		StatsBarWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AXYXBaseNPC::HideStatsWidget()
{
	if (StatsBarWidget)
	{
		StatsBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AXYXBaseNPC::Death()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetState(EState::EDead);

		HandleMeshOnDeath();

		if (GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, nullptr);
		}

		if (BaseAIController)
			BaseAIController->StopMovement();


		if (GetCapsuleComponent())
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

		if (BaseAIController && BaseAIController->BrainComponent)
		{
			BaseAIController->BrainComponent->StopLogic("");
		}

		HideStatsWidget();

		if (CollisionHandlerComp)
		{
			CollisionHandlerComp->DeactiveCollision();
		}

		SetLifeSpan(8.0f);
	}
}

void AXYXBaseNPC::Stunned()
{
	if (BaseAIController)
		BaseAIController->StopMovement();

	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, nullptr);
		UAnimMontage* Montage = GetStunMontage(ReceivedHitDirection);
		if (Montage)
		{
			float Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.f, EMontagePlayReturnType::Duration);
			if (Duration > 0.f && EffectsManagerComp)
			{
				EffectsManagerComp->AdjustEffectTime(EEffectType::EStun, Duration);
			}
		}
	}
}

void AXYXBaseNPC::Block()
{
	UAnimMontage* Montage = GetBlockMontage();
	if (Montage && GetMesh()->GetAnimInstance())
	{
		float Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.f, EMontagePlayReturnType::Duration);
	}
}

void AXYXBaseNPC::Parried()
{
	if (BaseAIController)
		BaseAIController->StopMovement();

	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, nullptr);
		UAnimMontage* Montage = GetParriedMontage();
		if (Montage)
		{
			float Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.f, EMontagePlayReturnType::Duration);
			if (Duration > 0.f && EffectsManagerComp)
			{
				EffectsManagerComp->AdjustEffectTime(EEffectType::EParried, Duration);
			}
		}
	}
}

void AXYXBaseNPC::Impact()
{
	if (BaseAIController)
		BaseAIController->StopMovement();

	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, nullptr);
		UAnimMontage* Montage = GetImpactMontage();
		if (Montage)
		{
			float Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.f, EMontagePlayReturnType::Duration);
			if (Duration > 0.f && EffectsManagerComp)
			{
				EffectsManagerComp->AdjustEffectTime(EEffectType::EImpact, Duration);
			}
		}
	}
}

void AXYXBaseNPC::Backstabbed()
{
	if (BaseAIController)
		BaseAIController->StopMovement();

	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, nullptr);
		if (MontageManagerComp)
		{
			UAnimMontage* Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EBackstabbed, 0);
			if (Montage)
			{
				float Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.f, EMontagePlayReturnType::Duration);
				if (Duration > 0.f && EffectsManagerComp)
				{
					EffectsManagerComp->AdjustEffectTime(EEffectType::EBackstab, Duration);
				}
			}
		}
	}
}

FHitData AXYXBaseNPC::MakeMeleeHitData(AActor* HitActor)
{
	FHitData TmpData; 
	if (HitActor && StatsManagerComp)
	{
		TmpData.Damage = StatsManagerComp->GetDamage();
		TmpData.DamageCauser = this;
		TmpData.HitFromDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), HitActor->GetActorLocation());
		TmpData.CanBeBlocked = true;
		TmpData.CanBeParried = true;
		TmpData.CanReceivedImpact = MeleeAttackType == EMeleeAttackType::ELight;
	}

	return TmpData;
}

void AXYXBaseNPC::ApplyHitImpulseToCharacter(AActor* HitActor, FVector HitNormal, float ImpulsePower)
{
	AXYXCharacter* XYXCharacter = Cast<AXYXCharacter>(HitActor);
	if (XYXCharacter)
	{
		if (XYXCharacter->GetMesh() && XYXCharacter->GetMesh()->IsAnySimulatingPhysics())
		{
			XYXCharacter->GetMesh()->AddImpulse(HitNormal * -1.f * ImpulsePower, TEXT("None"), false);
		}
	}
}

float AXYXBaseNPC::MeleeAttack(EMeleeAttackType Type)
{
	MeleeAttackType = Type;			
	float Duration = 0.f;
	if (StateManagerComp)
	{
		StateManagerComp->SetState(EState::EAttacking);

		UWorld* World = GetWorld();
		check(World);

		World->GetTimerManager().ClearTimer(ResetMeleeAttackCounterTimer);

		UAnimMontage* Montage = GetMeleeAttackMontage(Type);
		if (Montage && GetMesh()->GetAnimInstance() && StatsManagerComp)
		{
			float AttckSpeed = StatsManagerComp->GetStatValue(EAttributesType::EAttackSpeed, true);
			Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, AttckSpeed, EMontagePlayReturnType::Duration);
			if (Duration > 0.f && EffectsManagerComp)
			{
				EffectsManagerComp->AdjustEffectTime(EEffectType::EBackstab, Duration);
				if (StateManagerComp)
				{
					StateManagerComp->ResetState(Duration);

					float TmpValue = StatsManagerComp->GetStatValue(EAttributesType::EMeleeAttackStaminaCost, true);
					TmpValue = UXYXFunctionLibrary::ScaleMeleeAttackStaminaCostByType(TmpValue, Type);
					if (ExtendedStamina)
					{
						ExtendedStamina->ModifyStat(TmpValue * -1.f, true);
					}

					World->GetTimerManager().SetTimer(ResetMeleeAttackCounterTimer, this, &AXYXBaseNPC::ResetMeleeAttackCounter, Duration * 1.5f, false);
				}
			}
		}
		else
		{
			if (StateManagerComp)
			{
				StateManagerComp->ResetState(0.f);
			}
		}
	}
	return Duration;
}

UAnimMontage* AXYXBaseNPC::GetMeleeAttackMontage(EMeleeAttackType Type)
{
	EMontageAction TmpAction = UXYXFunctionLibrary::ConvertMeleeAttackTypeToAction(Type);
	int32 TmpLastIndex = 0;
	UAnimMontage* Montage = nullptr;
	if (MontageManagerComp)
	{
		TmpLastIndex = MontageManagerComp->GetMontageActionLastIndex(TmpAction);
		Montage = MontageManagerComp->GetMontageForAction(TmpAction,
			MeleeAttackCounter > TmpLastIndex ? TmpLastIndex : MeleeAttackCounter);
		if (Montage)
		{
			MeleeAttackCounter++;
			if (MeleeAttackCounter > TmpLastIndex)
			{
				MeleeAttackCounter = 0;
			}
		}
	}

	return Montage;
}

void AXYXBaseNPC::ResetMeleeAttackCounter()
{
	MeleeAttackCounter = 0;
}

float AXYXBaseNPC::Roll(EDirection Direction)
{
	float Duration = 0.f;
	if (StateManagerComp)
	{
		StateManagerComp->SetState(EState::ERolling);
	}

	UAnimMontage* Montage = GetRollMontage(Direction);
	if (Montage)
	{
		Duration = PlayAnimMontage(Montage);
		if (Duration > 0.f && StateManagerComp) 
		{
			StateManagerComp->ResetState(Duration);
		}
	}
	else
	{
		if (StateManagerComp)
		{
			StateManagerComp->ResetState(0.f);
		}
	}

	return Duration;
}

void AXYXBaseNPC::HandleOnCollisionActivated(ECollisionPart Selection)
{
	switch (Selection)
	{
	case ECollisionPart::EMainHandItem:
	{
		// Set Main Hand Item as new Collision mesh
		if (EquipmentComp)
		{
			AXYXDisplayedItem* DIOne = nullptr;
			AXYXDisplayedItem* DISecond = nullptr;
			EquipmentComp->GetDisplayedItem(
				EquipmentComp->GetSelectedMainHandType(), EquipmentComp->GetSelectedMainHandSlotIndex(), DIOne, DISecond);

			if (DIOne && CollisionHandlerComp)
			{
				UPrimitiveComponent* StaticMeshComp = DIOne->GetPrimaryComponent();
				CollisionHandlerComp->SetCollisionMesh(StaticMeshComp, StaticMeshComp->GetAllSocketNames());
			}
		}
	}
	break;
	case ECollisionPart::ELeftHand:
	{
		if (CollisionHandlerComp)
		{
			CollisionHandlerComp->SetCollisionMesh(GetMesh(), LeftHandCollisionSockets);
		}
	}
	break;
	case ECollisionPart::ERightHand:
	{
		if (CollisionHandlerComp)
		{
			CollisionHandlerComp->SetCollisionMesh(GetMesh(), RightHandCollisionSockets);
		}
	}
	break;
	case ECollisionPart::ELeftFoot:
	{
		if (CollisionHandlerComp)
		{
			CollisionHandlerComp->SetCollisionMesh(GetMesh(), LeftFootCollisionSockets);
		}
	}
	break;
	case ECollisionPart::ERightFoot:
	{
		if (CollisionHandlerComp)
		{
			CollisionHandlerComp->SetCollisionMesh(GetMesh(), RightFootCollisionSockets);
		}
	}
	break;
	}
}

bool AXYXBaseNPC::CanBeInterrupted()
{
	if (StateManagerComp )
	{
		return  !StateManagerComp->GetActivityValue(EActivity::ECanBeInterrupted);
	}

	return true;
}

void AXYXBaseNPC::UpdateReceivedHitDirection(FVector HitFromDirection)
{
	ReceivedHitDirection = UXYXFunctionLibrary::GetHitDirection(HitFromDirection, this);
}


void AXYXBaseNPC::HandleMeshOnDeath()
{
	 GetAttachedActors(AttachedActors, true);

	 // Enable physics on Mesh
	 GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"), true);
	 GetMesh()->SetSimulatePhysics(true);

	 // Enable Physics on main hand/shield items 
	 if (EquipmentComp && EquipmentComp->GetIsInCombat())
	 {
		 AXYXDisplayedItem* DIOne = nullptr;
		 AXYXDisplayedItem* DISecond = nullptr;
		 EquipmentComp->GetDisplayedItem(
			 EquipmentComp->GetSelectedMainHandType(), EquipmentComp->GetSelectedMainHandSlotIndex(), DIOne, DISecond);
		 if (DIOne)
			 DIOne->SimulatePhysics();
		 if (DISecond)
			 DISecond->SimulatePhysics();
	 }

	 // Dissolve all attached actors and character mesh
}

void AXYXBaseNPC::HandleExtendedHealthOnValueChanged(float NewValue, float MaxValue)
{
	if (NewValue <= 0.f)
	{
		Death();
	}
}

void AXYXBaseNPC::HandleOnEffectApplied(EEffectType Type)
{
	switch (Type)
	{
	case EEffectType::EStun:
	{
		Stunned();
	}
	break;
	case EEffectType::EBackstab:
	{
		Backstabbed();
	}
	break;
	case EEffectType::EParried:
	{
		Parried();
	}
	break;
	case EEffectType::EImpact:
	{
		Impact();
	}
	break;
	}

	if (StateManagerComp)
	{
		StateManagerComp->SetState(EState::EDisabled);
	}
}

void AXYXBaseNPC::InitializeStatsWidget()
{
	if (StatsBarWidget)
	{
		StatsBarWidget->InitializeHealth(ExtendedHealth);
		StatsBarWidget->InitializeStamina(ExtendedStamina);
	}
}
