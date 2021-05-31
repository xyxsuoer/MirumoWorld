// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Game/XYXData.h"
#include <GenericTeamAgentInterface.h>
#include <Interfaces/XYXInterfaceTargetable.h>
#include <Interfaces/XYXInterfaceEntity.h>
#include <Interfaces/XYXInterfaceMontageManagerComp.h>
#include "XYXBaseNPC.generated.h"

UCLASS()
class MIRUMOWORLD_API AXYXBaseNPC : public ACharacter,
	public IGenericTeamAgentInterface, public IXYXInterfaceTargetable,
	public IXYXInterfaceEntity, public IXYXInterfaceMontageManagerComp
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXYXBaseNPC();

protected:

	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXMontageManagerComponent* MontageManagerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXStateManagerComponent* StateManagerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXMovementSpeedComponent* MovementSpeedComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXEquipmentManagerComponent* EquipmentComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXRotatingComponent* RotatingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXCollisionHandlerComponent* CollisionHandlerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXEffectsComponent* EffectsManagerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXMontageManagerComponent* GetMontageManagerComponent() const { return MontageManagerComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXStateManagerComponent* GetStateManagerComponent() const { return StateManagerComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXMovementSpeedComponent* GetMovementSpeedComponent() const { return MovementSpeedComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXEquipmentManagerComponent* GetEquipmentManagerComponent() const { return EquipmentComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXRotatingComponent* GetRotatingComponent() const { return RotatingComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXCollisionHandlerComponent* GetCollisionHandlerComponent() const { return CollisionHandlerComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXEffectsComponent* GetEffectsComponent() const { return EffectsManagerComp; }

	UPROPERTY(Replicated)
		ETeam CombatTeam = ETeam::EEnemiesTeam;

	UFUNCTION(BlueprintPure, Category = XYX)
		class UXYXAnimInstance* GetXYXAnimInstance() const;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		int32 MeleeAttackCounter = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EMeleeAttackType MeleeAttackType = EMeleeAttackType::ENone;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EDirection ReceivedHitDirection = EDirection::EFront;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FName> LeftHandCollisionSockets = { TEXT("left_hand_1"), TEXT("left_hand_2") };

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FName> RightHandCollisionSockets = { TEXT("right_hand_1"), TEXT("right_hand_2") };

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FName> RightFootCollisionSockets = { TEXT("right_foot_1"), TEXT("right_foot_2") };

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FName> LeftFootCollisionSockets = { TEXT("left_foot_1"), TEXT("left_foot_2") };

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float RecentlyReceivedDamageStunLimit = 40.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		int32 RecentlyReceivedHitsCountStunLimit = 2;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		TArray<AActor*> AttachedActors;

public:
 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		UDataTable* GetMontages(EMontageAction Action);
	virtual UDataTable* GetMontages_Implementation(EMontageAction Action);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		ETeam GetEntityCombatTeam();
	virtual ETeam GetEntityCombatTeam_Implementation() override {
		return CombatTeam;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool IsEntityAlive();
	virtual bool IsEntityAlive_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		FName GetHeadSocket();
	virtual FName GetHeadSocket_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		FRotator GetDesiredRotation();
	virtual FRotator GetDesiredRotation_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool TakeAttackDamage(FHitData HitData, EAttackResult& ResultType);
	virtual bool TakeAttackDamage_Implementation(FHitData HitData, EAttackResult& ResultType) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool CanEffectBeApplied(EEffectType Type, AActor* Applier);
	virtual bool CanEffectBeApplied_Implementation(EEffectType Type, AActor* Applier) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool OnSelected();
	virtual bool OnSelected_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool OnDeselected();
	virtual bool OnDeselected_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool IsTargetable();
	virtual bool IsTargetable_Implementation() override;

	UFUNCTION()
		UAnimMontage* GetRollMontage(EDirection Direction);

	UFUNCTION()
		UAnimMontage* GetBlockMontage();

	UFUNCTION()
		UAnimMontage* GetImpactMontage();

	UFUNCTION()
		UAnimMontage* GetParriedMontage();

	UFUNCTION()
		bool CanBeStunned();

	UFUNCTION()
		bool CanBeAttacked();

	UFUNCTION()
		bool CanBeBackstabbed();

	UFUNCTION()
		void ShowStatsWidget();

	UFUNCTION()
		void HideStatsWidget();

	UFUNCTION()
		void Death();

	UFUNCTION()
		void Stunned();

	UFUNCTION()
		void Block();

	UFUNCTION()
		void Parried();

	UFUNCTION()
		void Impact();

	UFUNCTION()
		void Backstabbed();

	UFUNCTION()
		FHitData MakeMeleeHitData(AActor* HitActor);

	UFUNCTION()
		void ApplyHitImpulseToCharacter(AActor* HitActor, FVector HitNormal, float ImpulsePower);

	UFUNCTION()
		float MeleeAttack(EMeleeAttackType Type);

	UFUNCTION()
		UAnimMontage* GetMeleeAttackMontage(EMeleeAttackType Type);

	UFUNCTION()
		void ResetMeleeAttackCounter();

	UFUNCTION()
		float Roll(EDirection Direction);

	UFUNCTION()
		void OnCollisionActivated(ECollisionPart Selection);

	UFUNCTION()
		bool CanBeInterrupted();

};
