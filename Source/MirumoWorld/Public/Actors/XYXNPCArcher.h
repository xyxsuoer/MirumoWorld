// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/XYXBaseNPC.h"
#include "Interfaces/XYXInterfaceArcher.h"
#include "XYXNPCArcher.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API AXYXNPCArcher : public AXYXBaseNPC,
	public IXYXInterfaceArcher
{
	GENERATED_BODY()

public:

	AXYXNPCArcher();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class USceneComponent* ArrowSpawnLocation;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float AimAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = XYX)
		float ArrowInitialSpeed = 3500.f;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		float GetAimAlpha();
	virtual float GetAimAlpha_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool DoesHoldBowString();
	virtual bool DoesHoldBowString_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		FName GetBowStringSocketName();
	virtual FName GetBowStringSocketName_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		void ShootArrowProjectile();
	virtual void ShootArrowProjectile_Implementation();

	UFUNCTION()
		void UpdateAimAlpha();

	UFUNCTION(BlueprintCallable, Category = XYX)
		float BowAttack();

	UFUNCTION()
		FTransform GetSpawnedArrowTranform();

	UFUNCTION()
		bool CanBowAttack();

	UFUNCTION()
		void HandleOnActivityChanged(EActivity Activity, bool Value);
};
