// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Game/XYXData.h"
#include "XYXMeleeAIAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXMeleeAIAnimInstance : public UAnimInstance
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
		bool bIsBlocking = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float BlockAlpha = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		bool bIsInCombat = false;

	UPROPERTY(BlueprintReadOnly, Category = ACF)
		class UCharacterMovementComponent* MovementComp;

	UPROPERTY(BlueprintReadOnly, Category = ACF)
		class UXYXEquipmentManagerComponent* EquipmentComp;

	UPROPERTY(BlueprintReadOnly, Category = ACF)
		class UXYXStateManagerComponent* StateManagerComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EWeaponType CurWeaponType = EWeaponType::ENone;

public:

	UFUNCTION()
		void HandleOnInCombatChanged(bool bInCombat);

	UFUNCTION()
		void HandleOnActivityChanged(EActivity Activity, bool Value);
	
};
