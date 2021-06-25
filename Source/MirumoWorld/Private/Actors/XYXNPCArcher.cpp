// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/XYXNPCArcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include <Game/XYXData.h>
#include "Game/XYXGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Components/XYXStateManagerComponent.h"
#include "Items/ObjectItems/XYXItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actors/XYXArrowProjectileBase.h"
#include "Actors/XYXBaseAIController.h"
#include <Items/ObjectItems/XYXItemArrow.h>
#include "Components/XYXStatsManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/XYXMontageManagerComponent.h"

AXYXNPCArcher::AXYXNPCArcher()
{
	PrimaryActorTick.bCanEverTick = true;

	if (GetMesh())
	{
		ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSapwnLocation"));
		ArrowSpawnLocation->SetupAttachment(GetMesh());
	}
}

void AXYXNPCArcher::BeginPlay()
{
	Super::BeginPlay();

	if (EquipmentComp)
	{
		EquipmentComp->SetMainHandType(EItemType::ERangeWeapon);
		EquipmentComp->ChangeShootMode();
	}

	if (StateManagerComp)
	{
		StateManagerComp->OnActivityChanged.AddDynamic(this, &AXYXNPCArcher::HandleOnActivityChanged);
	}
}

void AXYXNPCArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAimAlpha();
}

float AXYXNPCArcher::GetAimAlpha_Implementation()
{
	return AimAlpha;
}

bool AXYXNPCArcher::DoesHoldBowString_Implementation()
{
	if (!EquipmentComp ||!StateManagerComp || !GetMesh() || !GetMesh()->GetAnimInstance() ||
		GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return false;
	}

	if (!GetCharacterMovement()->IsFalling() && StateManagerComp->GetState() != EState::EIdle &&
		EquipmentComp->GetIsInCombat() && EquipmentComp->GetCombatType() == ECombatType::ERanged)
	{
		if (!EquipmentComp->bActionShootOrAimShoot)
		{
			return true;
		}
	}

	return false;
}

FName AXYXNPCArcher::GetBowStringSocketName_Implementation()
{
	return TEXT("bow_string");
}

void AXYXNPCArcher::ShootArrowProjectile_Implementation()
{
	if (EquipmentComp)
	{
		auto TmpItem = EquipmentComp->GetActiveItem(EItemType::EArrows, 0);
		TSubclassOf<class UXYXItemBase> ItemArrowClass = TmpItem.ItemClass;
		if (UKismetSystemLibrary::IsValidClass(ItemArrowClass))
		{
			UXYXItemArrow* ItemArrow = NewObject<UXYXItemArrow>(this, ItemArrowClass);
			if (UKismetSystemLibrary::IsValidClass(ItemArrow->GetProjectile()))
			{
				FTransform TmpTransform = GetSpawnedArrowTranform();
				UWorld* const World = GetWorld();
				check(World);
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				ActorSpawnParams.Owner = this;
				ActorSpawnParams.Instigator = this;
				AXYXArrowProjectileBase* XYXActor = World->SpawnActor<AXYXArrowProjectileBase>(ItemArrow->GetProjectile(), TmpTransform, ActorSpawnParams);
				if (XYXActor && StatsManagerComp)
				{
					float TmpValue = EquipmentComp->bActionShootOrAimShoot ? 1 : AimAlpha;
					XYXActor->Damage = TmpValue * StatsManagerComp->GetDamage();
					XYXActor->InitialSpeed = TmpValue * ArrowInitialSpeed;
				}
			}
		}
	}
}

void AXYXNPCArcher::UpdateAimAlpha()
{
	UWorld* const World = GetWorld();
	if (World && EquipmentComp && !EquipmentComp->bActionShootOrAimShoot && StateManagerComp)
	{
		float AttackSpeed = StatsManagerComp ? StatsManagerComp->GetStatValue(EAttributesType::EAttackSpeed, true) : 1.0f;

		float To0 = UKismetMathLibrary::FInterpTo_Constant(AimAlpha, 0.f, World->GetDeltaSeconds(), 3.0f);
		float To1 = UKismetMathLibrary::FInterpTo_Constant(AimAlpha, 1.f, World->GetDeltaSeconds(), AttackSpeed);
		bool TmpCondition = StateManagerComp->GetActivityValue(EActivity::EIsAmingPressed) && StateManagerComp->GetState() == EState::EIdle; 
		AimAlpha = TmpCondition ? To1 : To0;
	}
}

float AXYXNPCArcher::BowAttack()
{
	if (!CanBowAttack())
	{
		return 0.f;
	}

	Execute_ShootArrowProjectile(this);

	if (StateManagerComp && MontageManagerComp)
	{
		StateManagerComp->SetState(EState::EAttacking);
		auto Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EShootArrow, 0);
		if (Montage)
		{
			auto Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f, EMontagePlayReturnType::Duration);
			if (StateManagerComp)
			{
				StateManagerComp->ResetState(Duration);
				return Duration;
			}
		}
		else
		{
			StateManagerComp->ResetState(0.f);
		}
	}

	return 0.f;
}

FTransform AXYXNPCArcher::GetSpawnedArrowTranform()
{
	if (!BaseAIController || !BaseAIController->Target || !ArrowSpawnLocation)
	{
		return FTransform::Identity;
	}

	FVector TmpArrowSpawnLocation = ArrowSpawnLocation->GetComponentLocation();
	FVector TmpAIControllerLocation = BaseAIController->Target->GetActorLocation();
	FVector TmpAIControllerVelocity = BaseAIController->Target->GetVelocity();
	// Add some randomness to not be 100% accurate
	float TmpRandomF1 = UKismetMathLibrary::RandomFloatInRange(-35.f, 35.f);
	float TmpRandomF2 = UKismetMathLibrary::RandomFloatInRange(-25.f, 50.f);
	FVector TmpVector = FVector(TmpAIControllerLocation.X + TmpRandomF1, 
		TmpAIControllerLocation.Y + TmpRandomF1, TmpAIControllerLocation.Z + TmpRandomF2);
	float ToTargetDistance = this->GetDistanceTo(BaseAIController->Target);
	FVector TmpVector2 = TmpAIControllerVelocity * (ToTargetDistance / ArrowInitialSpeed);
	FRotator TmpLookAtRotation = UKismetMathLibrary::FindLookAtRotation(TmpArrowSpawnLocation, TmpVector + TmpVector2);

	return FTransform(TmpLookAtRotation, TmpArrowSpawnLocation, FVector(1.f, 1.f, 1.f));
}

bool AXYXNPCArcher::CanBowAttack()
{
	if (EquipmentComp && EquipmentComp->GetIsInCombat() && EquipmentComp->AreArrowEquipped() &&
		EquipmentComp->GetCombatType() == ECombatType::ERanged)
	{
		return true;
	}

	return false;
}

void AXYXNPCArcher::HandleOnActivityChanged(EActivity Activity, bool Value)
{
	if (Activity == EActivity::EIsAmingPressed && Value && GetWorld())
	{
		UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(GetWorld()->GetGameInstance());
		if (!GameInstance || !GetMesh() || !GameInstance->SoundDataTable)
		{
			return;
		}

		USoundBase* TmpSound = nullptr;
		FSoundFX* pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("DrawBow"), "");
		if (pRow)
		{
			TmpSound = pRow->SoundBase;
		}

		if (TmpSound)
		{
			UGameplayStatics::SpawnSoundAttached(TmpSound, GetMesh());
		}
	}
}
