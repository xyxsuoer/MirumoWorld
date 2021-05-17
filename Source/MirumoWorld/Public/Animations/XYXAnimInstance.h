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

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		bool bIsInCombat = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsTwoHandedWeaponEquipped = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		ECombatType CurCombatType = ECombatType::EUnarmed;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EWeaponType CurWeaponType = EWeaponType::ENone;

	// Lean
	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float LeanAmount = 0.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = XYX)
		float LeanOffset = 10.f;

	UPROPERTY(BlueprintReadOnly, Category = ACF)
		class UCharacterMovementComponent* MovementComp;

	UPROPERTY(BlueprintReadOnly, Category = ACF)
		class UXYXEquipmentManagerComponent* EquipmentComp;

	UPROPERTY(BlueprintReadOnly, Category = ACF)
		class UXYXStateManagerComponent* StateManagerComp;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FRotator Rotation;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EMovementState MovementState = EMovementState::EIdle;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsCrouching = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsBlocking = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsBowActionMode = false;

	UFUNCTION()
		void HandleOnInCombatChanged(bool bInCombat);

	UFUNCTION()
		void HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex);

	UFUNCTION()
		void HandleOnMainHandTypeChanged(EItemType Type);

	UFUNCTION()
		void HandleOnCombatTypeChanged(ECombatType CombatType);

	UFUNCTION()
		void HandleOnWeaponTypeChanged(EWeaponType WeaponType);

	UFUNCTION()
		void UpdateHandItemsInfo();

	UFUNCTION()
		void HandleOnActivityChanged(EActivity Activity, bool Value);

	UFUNCTION()
		void UpdateLeanAmount();

	UFUNCTION()
		void UpdateLookatValues();

	UFUNCTION()
		void UpdateAimOffsetAlpha();

private:

	void StoreCharacterInfo();

	void SetReferences();
};
