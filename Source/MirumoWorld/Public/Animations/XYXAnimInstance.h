// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Game/XYXData.h"
#include "XYXAnimInstance.generated.h"



UCLASS()
class MIRUMOWORLD_API UXYXAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	friend class AXYXCharacter;

	/* begin play */
	virtual void NativeInitializeAnimation() override;
	/* tick */
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	// Movement
	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class AXYXCharacter* XYXCharacter;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float Direction = 0.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = XYX)
		float LocomotionRateScale = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float LookAtPitch = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float LookAtYaw = 0.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = XYX)
		float TurnThreshold = 0.2f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = XYX)
		float MinTurnLength = 0.9f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float AimOffsetAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float AxisTurnValue = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float MouseDeltaX = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float CastingAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsInSlowMotion = false;

	// Combat
	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsItemInShieldSlotEquipped = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsShieldEquipped = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float BlockAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float AimAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsLookingForward = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsInCombat = true;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsTwoHandedWeaponEquipped = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		ECombatType combatType = ECombatType::EUnarmed;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EWeaponType weaponType = EWeaponType::ENone;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EKatanaStance KatanaStance = EKatanaStance::EKatanaS1;

	// Lean
	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float LeanAmount = 0.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = XYX)
		float LeanOffset = 10.f;

	UPROPERTY(BlueprintReadOnly, Category = ACF)
		class UCharacterMovementComponent* MovementComp;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FRotator Rotation;

private:

	void StoreCharacterInfo();

	void SetReferences();
};
