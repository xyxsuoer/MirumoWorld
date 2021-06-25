// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "XYXArcherAIAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXArcherAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	friend class AXYXBaseNPC;

	/* begin play */
	virtual void NativeInitializeAnimation() override;
	/* tick */
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:

	// Movement
	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class AXYXBaseNPC* CharacterReference;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float Direction = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float AimAlpha = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		bool bIsInCombat = false;

	UPROPERTY(BlueprintReadOnly, Category = ACF)
		class UCharacterMovementComponent* MovementComp;

	UPROPERTY(BlueprintReadOnly, Category = ACF)
		class UXYXEquipmentManagerComponent* EquipmentComp;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float AimOffsetAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float LookAtYaw = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float LookAtPitch = 0.f;

public:

	UFUNCTION()
		void HandleOnInCombatChanged(bool bInCombat);

};
