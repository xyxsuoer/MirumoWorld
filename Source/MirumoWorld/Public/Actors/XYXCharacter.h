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

#define ECC_XYXProjectileChannel		ECollisionChannel::ECC_GameTraceChannel1
#define ECC_XYXInteractableChannel  ECollisionChannel::ECC_GameTraceChannel2

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXRotatingComponent* RotatingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXDynamicTargetingComponent* DynamicTargetingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UArrowComponent* TargetArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXCollisionHandlerComponent* CollisionHandlerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXEffectsComponent* EffectsManagerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXStatsManagerComponent* StatsManagerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXExtendedStatComponent* ExtendedHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXExtendedStatComponent* ExtendedStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXExtendedStatComponent* ExtendedMana;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class USceneComponent* ArrowSpawnLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UAudioComponent* EffectsAudio;

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

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXRotatingComponent* GetRotatingComponent() const { return RotatingComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXDynamicTargetingComponent* GetDynamicTargetingComponent() const { return DynamicTargetingComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXCollisionHandlerComponent* GetCollisionHandlerComponent() const { return CollisionHandlerComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXEffectsComponent* GetEffectsComponent() const { return EffectsManagerComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXStatsManagerComponent* GetStatsManagerComponent() const { return StatsManagerComp; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXExtendedStatComponent* GetExtendedHealth() const { return ExtendedHealth; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXExtendedStatComponent* GetExtendedStamina() const { return ExtendedStamina; }

	UFUNCTION(BlueprintPure, Category = XYX)
		FORCEINLINE class UXYXExtendedStatComponent* GetExtendedMana() const { return ExtendedMana; }

	// =================================================

	UPROPERTY(EditDefaultsOnly, Category = XYX)
		FName CharacterName = TEXT("SaulXu");

	UPROPERTY(Replicated)
		ETeam CombatTeam = ETeam::EHeroesTeam;

	UFUNCTION(BlueprintPure, Category = XYX)
		class UXYXAnimInstance* GetXYXAnimInstance() const;

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

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FName> SpineCollisionSockets = { TEXT("left_spine"), TEXT("right_spine") };

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

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float CrouchStaminaCost = 0.1f;

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

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		UCurveFloat* BlockingFloatCurve;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TEnumAsByte<ETimelineDirection::Type> BlockingTimelineDirection;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		UTimelineComponent* ZoomingTimeline;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		UCurveFloat* ZoomingFloatCurve;

	UPROPERTY(EditAnywhere, Category = XYX)
		TSubclassOf<class UXYXUserWidgetInGame> WBInGameClass;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		class UXYXUserWidgetInGame* WBInGame;

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

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode) override;

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
			FName GetHeadSocket();
		virtual FName GetHeadSocket_Implementation();

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

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
			FRotator GetDesiredRotation();
		virtual FRotator GetDesiredRotation_Implementation() override;

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
			bool TakeAttackDamage(FHitData HitData, EAttackResult& ResultType);
		virtual bool TakeAttackDamage_Implementation(FHitData HitData, EAttackResult& ResultType) override;

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
			bool CanEffectBeApplied(EEffectType Type, AActor* Applier);
		virtual bool CanEffectBeApplied_Implementation(EEffectType Type, AActor* Applier) override;

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
		void HandleOnRotatingEnd();

	UFUNCTION()
		void HandleOnTargetingToggled(bool bEnabled);

	UFUNCTION()
		void CombatAttack(EMeleeAttackType AttackType);

	UFUNCTION()
		void MeleeAttack(EMeleeAttackType AttackType);

	UFUNCTION()
		void BowActionAttack(EMeleeAttackType AttackType);

	UFUNCTION()
		void PlayBowActionAttackMontage();

	UFUNCTION()
		bool CanMeleeAttack();

	UFUNCTION()
		bool CanBowAttack();

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

	UFUNCTION()
		void StartBowAimModeAttackAction();

	UFUNCTION()
		void EndBowAimModeAttackAction();

	UFUNCTION()
		void ShootArrow();
	
	UFUNCTION()
		void AttemptPlayBowDrawSound();

	UFUNCTION()
		void PlayBowDrawSound();

	UFUNCTION()
		void StopBowDrawSound();

	UFUNCTION()
		void StartAiming();

	UFUNCTION()
		void StopAiming();

	UFUNCTION()
		void StartLookingForward();

	UFUNCTION()
		void StopLookingForward();

	UFUNCTION()
		void StartZooming();

	UFUNCTION()
		void StopZooming();

	UFUNCTION()
		void UpdateRotationSettings();

	UFUNCTION()
		void UpdateZooming();

	UFUNCTION()
		void RegisterZoomingTimeline();

	UFUNCTION()
		void ZoomingTimelineCallback(float Val);

	UFUNCTION()
		void ZoomingTimelineFinishedCallback();

	UFUNCTION()
		void PlayZoomingTimeline(bool bInPlay);

	UFUNCTION()
		void UpdateAimAlpha();

	UFUNCTION()
		FTransform GetSpawnedArrowTranform();

	UFUNCTION()
		void ResetAimingMode();

	UFUNCTION()
		void SetCameraLagSmoothly(float InTargetLagSpeed);

	UFUNCTION()
		void UpdateCameraLag();

	UFUNCTION()
		void CalculateLeanAmount(float& LeanAmount, float& InterpSpeed);

	UFUNCTION()
		void ShowCrosshair(UTexture2D* InTexture);

	UFUNCTION()
		void HideCrosshair();

	UFUNCTION()
		void UpdateCrosshairPosition();

	UFUNCTION()
		void Death();

	UFUNCTION()
		void HandleOnHit(FHitResult HitResult);

	UFUNCTION()
		void HandleOnCollisionActivated(ECollisionPart CollisionPart);

	UFUNCTION()
		FHitData MakeMeleeHitData(AActor* HitActor);

	UFUNCTION()
		void ApplyHitImpulseToCharacter(AActor* HitActor, FVector HitNormal, float ImpulsePower);

	UFUNCTION()
		bool CanBeAttacked();

	UFUNCTION()
		bool CanBeStunned();

	UFUNCTION()
		bool CanBeInterrupted();

	UFUNCTION()
		void UpdateReceivedHitDirection(FVector HitFromDirection);

	UFUNCTION()
		void Block();

	UFUNCTION()
		void HandleOnEffectApplied(EEffectType Type);

	UFUNCTION()
		void HandleOnEffectRemoved(EEffectType Type);

	UFUNCTION()
		void Stunned();

	UFUNCTION()
		void Backstabbed();

	UFUNCTION()
		void Impact();

	UFUNCTION()
		void Parried();

	UFUNCTION()
		bool AttemptBackstab();

	UFUNCTION()
		float GetCastingSpeed();

	UFUNCTION()
		float GetMagicDamage();

	UFUNCTION()
		void StartSlowMotion();

	UFUNCTION()
		void StopSlowMotion();

	UFUNCTION()
		void LoopSlowMotion();

	UFUNCTION()
		bool CanEnterSlowMotion();

	UFUNCTION()
		bool IsEnoughStamina(float Value);

	UFUNCTION()
		void HandleExtendedHealthOnValueChanged(float NewValue, float MaxValue);

	UFUNCTION()
		void HandleExtendedStaminaOnValueChanged(float NewValue, float MaxValue);

	UFUNCTION()
		void TargetLeftAction();

	UFUNCTION()
		void TargetRightAction();

	UFUNCTION()
		void ToggleTargetAction();

private:
	bool bInitialized = false;

	float HorizontalLookValue = 0.f;

	float DelayPlayBowActionAttackMontage = 0.28f;

	UPROPERTY()
		FTimerHandle ResetMeleeAttackCounterTimer;

	UPROPERTY()
		FTimerHandle SprintLoopTimer;

	UPROPERTY()
		FTimerHandle CrouchLoopTimer;

	UPROPERTY()
		FTimerHandle StopLookingForwardTimer;

	UPROPERTY()
		FTimerHandle UpdateCameraLagTimer;

	UPROPERTY()
		FTimerHandle HideCrosshairTimer;

	UPROPERTY()
		FTimerHandle LoopSlowMotionTimer;

	UPROPERTY()
		FTimerHandle PlayBowDrawSoundTimer;

	UPROPERTY()
		FTimerHandle ClearPlayBowDrawSoundTimer;

};
