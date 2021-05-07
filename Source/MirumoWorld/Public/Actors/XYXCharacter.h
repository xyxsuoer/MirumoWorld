// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Game/XYXData.h"
#include "GenericTeamAgentInterface.h"
#include "Interfaces/XYXInterfaceMontageManagerComp.h"
#include "Interfaces/XYXInterfaceEntity.h"
#include "Interfaces/XYXInterfaceArcher.h"
#include "Components/TimelineComponent.h"
#include "XYXCharacter.generated.h"

#define ECC_ACFHeroesChannel  ECollisionChannel::ECC_GameTraceChannel1
#define ECC_ACFEnemiesChannel  ECollisionChannel::ECC_GameTraceChannel2

UCLASS()
class MIRUMOWORLD_API AXYXCharacter : public ACharacter,
	public IGenericTeamAgentInterface, public IXYXInterfaceMontageManagerComp, public IXYXInterfaceEntity,
	public IXYXInterfaceArcher
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXYXCharacter(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void PostInitProperties() override;

	virtual void PreInitializeComponents() override;

	virtual void PostInitializeComponents() override;

	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Components ==========================================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXInputBufferComponent* InputBufferComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXMontageManagerComponent* MontageManagerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXStateManagerComponent* StateManagerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXMovementSpeedComponent* MovementSpeedComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXEquipmentManagerComponent* EquipmentComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXInventoryManagerComponent* InventoryComp;

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXInputBufferComponent* GetInputBufferComponent() const { return InputBufferComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXMontageManagerComponent* GetMontageManagerComponent() const { return MontageManagerComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXStateManagerComponent* GetStateManagerComponent() const { return StateManagerComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXMovementSpeedComponent* GetMovementSpeedComponent() const { return MovementSpeedComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXEquipmentManagerComponent* GetEquipmentManagerComponent() const { return EquipmentComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXInventoryManagerComponent* GetInventoryManagerComponent() const { return InventoryComp; }

	// =================================================

	UPROPERTY(EditDefaultsOnly, Category = XYX)
		FName CharacterName = TEXT("SaulXu");

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		bool bIsDead = false;

	UPROPERTY(Replicated)
		ETeam CombatTeam = ETeam::EHeroesTeam;

	UFUNCTION(BlueprintPure, Category = XYX)
		class UXYXAnimInstance* GetACFAnimInstance() const;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		int32 MeleeAttackCounter = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EMeleeAttackType MeleeAttackType = EMeleeAttackType::ENone;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float BlockAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class AActor* BackstabbedActor;

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
		float HorizontalLookRate = 45.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float VerticalLookRate = 45.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		EMovementState StoredMovementState = EMovementState::EIdle;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float RollStaminaCost = 25.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float SprintStaminaCost = 0.5f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsCrosshairVisible = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float AimAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsInSlowMotion = false;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float SlowMotionTimeDilation = 0.4f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float SlowMotionStaminaCost = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bAutoZoom = false;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		FCameraSettings ZoomedCameraSettings = FCameraSettings(FRotator(0.f, 0.f, 0.f), 150.f, FVector(0.f, 50.f, 100.f), 30.f);

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		FCameraSettings StartCameraSettings = FCameraSettings(FRotator(0.f, -15.f, 0.f), 450.f, FVector(0.f, 0.f, 180.f), 10.f);

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float ZoomAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float TargetLagSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsAbilityInputPressed = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsAbilityMainInputPressed = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		int32 SelectedSpellIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		UTimelineComponent* BlockingTimeline;

	UPROPERTY()
		UCurveFloat* BlockingFloatCurve;

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> BlockingTimelineDirection;

	// Movement Input
	virtual void MoveRight(float Val);

	virtual void MoveForward(float Val);

	// Camera Input
	void AddControllerYawInput(float Val);

	void AddControllerPitchInput(float Val);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//C++实现，蓝图调用
	UFUNCTION(BlueprintCallable, Category = XYX)
		bool GetIsInSlowMotion() { return bIsInSlowMotion; }

	UFUNCTION(BlueprintCallable, Category = XYX)
		float GetBlockAlpha() { return BlockAlpha; }

	UFUNCTION(BlueprintCallable, Category = XYX)
		void GetMovementVectors(FVector& ForwardVec, FVector& RightVec);

	// ======================= Interfaces===============================

	//C++实现默认版本，蓝图可override实现
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		UDataTable* GetMontages(EMontageAction Action );
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
			float GetAimAlpha();
		virtual float GetAimAlpha_Implementation();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
			bool DoesHoldBowString();
		virtual bool DoesHoldBowString_Implementation();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
			FName GetBowStringSocketName();
		virtual FName GetBowStringSocketName_Implementation();

		// =================================================================

	UFUNCTION()
		void InitialzeCharacter();

	UFUNCTION()
		void HandleInputBufferConsumed(const EInputBufferKey key);

	UFUNCTION()
		void HandleInputBufferClose();

	UFUNCTION()
		void HandleMovementStateStart(EMovementState State);

	UFUNCTION()
		void HandleMovementStateEnd(EMovementState State);

	UFUNCTION()
		void HandleOnInCombatChanged(bool bIsInCombat);

	UFUNCTION()
		void HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex);

	UFUNCTION()
		void HandleOnMainHandTypeChanged(EItemType Type);

	UFUNCTION()
		void HandleOnCombatTypeChanged(ECombatType CombatType);

	UFUNCTION()
		void HandleOnActivityChanged(EActivity Activity, bool Value);

	UFUNCTION()
		void HandleOnStateChanged(EState PrevState, EState NewState);

	UFUNCTION()
		void MeleeAttack(EMeleeAttackType AttackType);

	UFUNCTION()
		bool CanMeleeAttack();

	UFUNCTION()
		UAnimMontage* GetMeleeAttackMontage(const EMeleeAttackType AttackType);

	UFUNCTION()
		UAnimMontage* GetRollMontage();

	UFUNCTION()
		UAnimMontage* GetStunMontage(EDirection Direction);

	UFUNCTION()
		UAnimMontage* GetBlockMontage();

	UFUNCTION()
		UAnimMontage* GetImpactMontage();

	UFUNCTION()
		UAnimMontage* GetParriedMontage();

	UFUNCTION()
		UAnimMontage* GetParryMontage();

	UFUNCTION()
		void ResetMeleeAttackCounter() { MeleeAttackCounter = 0; }

	UFUNCTION()
		void LightAttack();

	UFUNCTION()
		void HeavyAttack();

	UFUNCTION()
		void ThrustAttack();

	UFUNCTION()
		void SpecialAttack();

	UFUNCTION()
		void RollAction();

	UFUNCTION()
		void JumpAction();

	UFUNCTION()
		void StopJumpAction();

	UFUNCTION()
		void ParryAction();

	UFUNCTION()
		void SprintAction();

	UFUNCTION()
		void StopSprintAction();

	UFUNCTION()
		bool WeaponCanSprint();

	UFUNCTION()
		void CrouchAction();

	UFUNCTION()
		void StopCrouchAction();

	UFUNCTION()
		void ToggleMovementAction();

	UFUNCTION()
		bool WeaponCanCrouch();

	UFUNCTION()
		void CustomJump();

	UFUNCTION()
		void Parry();

	UFUNCTION()
		void Roll();

	UFUNCTION()
		bool CanRoll();

	UFUNCTION()
		bool IsIdleAndNotFalling();

	UFUNCTION()
		bool IsStateEqual(EState StateToCompareWith);

	UFUNCTION()
		bool HasMovementInput();

	UFUNCTION()
		void SetSprintOrCrouch(bool bActivate, EMovementState MovementState);

	UFUNCTION()
		void SprintLoop();

	UFUNCTION()
		void CrouchLoop();

	UFUNCTION()
		bool CanUseOrSwitchItem();
		
	UFUNCTION()
		void SwitchMainHandTypeUpAction();

	UFUNCTION()
		void SwitchMainHandTypeDownAction();

	UFUNCTION()
		void SwitchMainHandType(bool bForward);

	UFUNCTION()
		void SwitchMainHandItemUpAction();

	UFUNCTION()
		void SwitchMainHandItemDownAction();

	UFUNCTION()
		void SwitchMainHandItem(bool bForward);

	UFUNCTION()
		void ToggleCombatAction();

	UFUNCTION()
		void ToggleCombat();

	UFUNCTION()
		void PlayMainHandTypeChangedMontage(EItemType Type);

	UFUNCTION()
		void BlockAction();

	UFUNCTION()
		void StopBlockAction();

	UFUNCTION()
		void UpdateBlocking();

	UFUNCTION()
		void RegisterBlockingTimeline();

	UFUNCTION()
		void BlockingTimelineCallback(float Val);

	UFUNCTION()
		void BlockingTimelineFinishedCallback();

	UFUNCTION()
		void PlayBlockingTimeline(bool bInPlay);

	UFUNCTION()
		void SwitchSomethingAction();

private:
	bool bInitialized = false;

	UPROPERTY()
		FTimerHandle ResetMeleeAttackCounterTimer;

	UPROPERTY()
		FTimerHandle SprintLoopTimer;

	UPROPERTY()
		FTimerHandle CrouchLoopTimer;



};
