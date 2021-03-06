// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXAnimInstance.h"
#include "Actors/XYXCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/NavMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/XYXMovementSpeedComponent.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Components/XYXStateManagerComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "Interfaces/XYXInterfaceArcher.h"

void UXYXAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SetReferences();
	
	if (IsValid(EquipmentComp))
	{
		EquipmentComp->OnInCombatChanged.AddDynamic(this, &UXYXAnimInstance::HandleOnInCombatChanged);
		EquipmentComp->OnActiveItemChanged.AddDynamic(this, &UXYXAnimInstance::HandleOnActiveItemChanged);
		EquipmentComp->OnMainHandTypeChanged.AddDynamic(this, &UXYXAnimInstance::HandleOnMainHandTypeChanged);
		EquipmentComp->OnCombatTypeChanged.AddDynamic(this, &UXYXAnimInstance::HandleOnCombatTypeChanged);
		EquipmentComp->OnWeaponTypeChanged.AddDynamic(this, &UXYXAnimInstance::HandleOnWeaponTypeChanged);
	}
	if (IsValid(StateManagerComp))
	{
		StateManagerComp->OnActivityChanged.AddDynamic(this, &UXYXAnimInstance::HandleOnActivityChanged);
	}
}

void UXYXAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (XYXCharacter)
	{
		StoreCharacterInfo();
		UpdateLeanAmount();
		UpdateLookatValues();
		UpdateAimOffsetAlpha();
	}
}

void UXYXAnimInstance::SetReferences()
{
	XYXCharacter = Cast<AXYXCharacter>(TryGetPawnOwner());
	if (XYXCharacter)
	{
		MovementComp = XYXCharacter->GetCharacterMovement();
		EquipmentComp = XYXCharacter->GetEquipmentManagerComponent();
		StateManagerComp = XYXCharacter->GetStateManagerComponent();
	}
}


void UXYXAnimInstance::HandleOnInCombatChanged(bool bInCombat)
{
	bIsInCombat = bInCombat;
}

void UXYXAnimInstance::HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex)
{
	UpdateHandItemsInfo();
}

void UXYXAnimInstance::HandleOnMainHandTypeChanged(EItemType Type)
{
	UpdateHandItemsInfo();
}

void UXYXAnimInstance::HandleOnCombatTypeChanged(ECombatType CombatType)
{
	CurCombatType = CombatType;
}

void UXYXAnimInstance::HandleOnWeaponTypeChanged(EWeaponType WeaponType)
{
	CurWeaponType = WeaponType;
}

void UXYXAnimInstance::UpdateHandItemsInfo()
{
	if (EquipmentComp )
	{
		bIsShieldEquipped = EquipmentComp->IsShieldEquipped();
		bIsTwoHandedWeaponEquipped = EquipmentComp->IsTwoHandedWeaponEquipped();
	}
}

void UXYXAnimInstance::HandleOnActivityChanged(EActivity Activity, bool Value)
{
	if (Activity == EActivity::EIsLookingForward)
	{
		bIsLookingForward = Value;
	}
}

void UXYXAnimInstance::UpdateLeanAmount()
{
	float TmpLeanAmount = 0;
	float TmpInterpSpeed = 1.f;
	if (XYXCharacter && GetWorld())
	{
		XYXCharacter->CalculateLeanAmount(TmpLeanAmount, TmpInterpSpeed);
		LeanAmount = UKismetMathLibrary::FInterpTo(LeanAmount, TmpLeanAmount, GetWorld()->GetDeltaSeconds(), TmpInterpSpeed);
	}
}

void UXYXAnimInstance::UpdateLookatValues()
{
	if (XYXCharacter && GetWorld())
	{
		FRotator TmpRotator = UKismetMathLibrary::NormalizedDeltaRotator(XYXCharacter->GetControlRotation(), XYXCharacter->GetActorRotation());
		FRotator TmpRotator2 = UKismetMathLibrary::MakeRotator(0.f, LookAtPitch, LookAtYaw);
		FRotator TmpRotator3 = UKismetMathLibrary::RInterpTo(TmpRotator, TmpRotator2, GetWorld()->GetDeltaSeconds(), 15.f);

		LookAtPitch = UKismetMathLibrary::ClampAngle(TmpRotator3.Pitch, -90.f, 90.f);
		LookAtYaw = UKismetMathLibrary::ClampAngle(TmpRotator3.Yaw, -90.f, 90.f);
	}
}

void UXYXAnimInstance::UpdateAimOffsetAlpha()
{
	if (GetWorld())
	{
		float TargetValue = bIsLookingForward ? 1.f : 0.f;
		AimOffsetAlpha = UKismetMathLibrary::FInterpTo(AimOffsetAlpha, TargetValue, GetWorld()->GetDeltaSeconds(), 5.f);
	}
}

void UXYXAnimInstance::StoreCharacterInfo()
{
	if (XYXCharacter && MovementComp)
	{
		bIsInAir = MovementComp->IsFalling();

		Velocity = XYXCharacter->GetVelocity();
		Speed = Velocity.Size();
		Rotation = XYXCharacter->GetActorRotation();
		Direction = CalculateDirection(Velocity, Rotation);

		bIsInSlowMotion = XYXCharacter->GetIsInSlowMotion();
		BlockAlpha = XYXCharacter->GetBlockAlpha();
		IXYXInterfaceArcher* ArcherCharacter = Cast<IXYXInterfaceArcher>(XYXCharacter);
		if (ArcherCharacter)
		{
			AimAlpha = ArcherCharacter->Execute_GetAimAlpha(XYXCharacter);
		}
		if (XYXCharacter->GetMovementSpeedComponent()) 
		{
			MovementState = XYXCharacter->GetMovementSpeedComponent()->GetMovementState();
			bIsSprinting =	MovementState == EMovementState::ESprint;
			bIsCrouching = MovementState == EMovementState::ECrouch;
		}

		if (IsValid(StateManagerComp))
		{
			bIsBlocking = StateManagerComp->GetActivityValue(EActivity::EIsBlockingPressed);
		}

		if (IsValid(EquipmentComp))
		{
			bIsBowActionMode = EquipmentComp->bActionShootOrAimShoot;
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			float MouseX = 0.0f;
			float MouseY = 0.0f;
			PlayerController->GetInputMouseDelta(MouseX, MouseY);
			MouseDeltaX = MouseX;
		}
	}
}


