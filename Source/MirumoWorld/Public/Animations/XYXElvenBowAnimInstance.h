// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "XYXElvenBowAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXElvenBowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	friend class AXYXCharacter;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class AXYXCharacter* XYXCharacter;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float AimAlpha = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FVector StringLocation;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FVector DefaultStringLocation;

	UFUNCTION()
		void UpdateStringLocation();
};
