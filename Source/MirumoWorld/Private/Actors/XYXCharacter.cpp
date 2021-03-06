// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/XYXCharacter.h"
#include "Animations/XYXAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/PhysicsFiltering.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Game/XYXFunctionLibrary.h"
#include "Game/XYXGameInstance.h"
#include "Components/XYXMontageManagerComponent.h"
#include "Components/XYXInputBufferComponent.h"
#include "Components/XYXStateManagerComponent.h"
#include "Components/XYXMovementSpeedComponent.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Components/XYXInventoryManagerComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actors/XYXArrowProjectileBase.h"
#include "Items/ObjectItems/XYXItemBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Items/ObjectItems/XYXItemArrow.h"
#include <Components/XYXRotatingComponent.h>
#include <Components/XYXDynamicTargetingComponent.h>
#include <Components/ArrowComponent.h>
#include "UI/XYXUserWidgetInGame.h"
#include <Components/XYXCollisionHandlerComponent.h>
#include <Components/XYXEffectsComponent.h>
#include "Components/XYXStatsManagerComponent.h"
#include "Components/XYXExtendedStatComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/DisplayedItems/XYXDisplayedItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystem.h"



// Sets default values
AXYXCharacter::AXYXCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->TargetArmLength = 450;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset.Z = 180.f;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	if (GetMesh())
	{
		ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSapwnLocation"));
		ArrowSpawnLocation->SetupAttachment(GetMesh());

		EffectsAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Effects Audio"));
		EffectsAudio->SetupAttachment(GetMesh());
	}

	TargetArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	TargetArrow->SetupAttachment(RootComponent);

	SetReplicates(true);
	SetReplicateMovement(true);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->RotationRate.Yaw = 540.f;
	GetCharacterMovement()->NavAgentProps.AgentRadius = 42.f;
	GetCharacterMovement()->NavAgentProps.AgentHeight = 192.f;

	// Components
	InputBufferComp = CreateDefaultSubobject<UXYXInputBufferComponent>(TEXT("Input Buffer Component"));
	MontageManagerComp = CreateDefaultSubobject<UXYXMontageManagerComponent>(TEXT("Montage Manager Component"));
	StateManagerComp = CreateDefaultSubobject<UXYXStateManagerComponent>(TEXT("State Manager Component"));
	MovementSpeedComp = CreateDefaultSubobject<UXYXMovementSpeedComponent>(TEXT("Movement Speed Component"));
	EquipmentComp = CreateDefaultSubobject<UXYXEquipmentManagerComponent>(TEXT("Equipment Manager Component"));
	InventoryComp = CreateDefaultSubobject<UXYXInventoryManagerComponent>(TEXT("Inventory Manager Component"));
	RotatingComp = CreateDefaultSubobject<UXYXRotatingComponent>(TEXT("Rotating Component"));
	DynamicTargetingComp = CreateDefaultSubobject<UXYXDynamicTargetingComponent>(TEXT("Dynamic Targeting Component"));
	CollisionHandlerComp = CreateDefaultSubobject<UXYXCollisionHandlerComponent>(TEXT("Collision Handler Component"));
	EffectsManagerComp = CreateDefaultSubobject<UXYXEffectsComponent>(TEXT("Effects Manager Component"));
	StatsManagerComp = CreateDefaultSubobject<UXYXStatsManagerComponent>(TEXT("Stats Manager Component"));
	ExtendedHealth = CreateDefaultSubobject<UXYXExtendedStatComponent>(TEXT("Extended Stat Health"));
	ExtendedStamina = CreateDefaultSubobject<UXYXExtendedStatComponent>(TEXT("Extended Stat Stamina"));
	ExtendedMana = CreateDefaultSubobject<UXYXExtendedStatComponent>(TEXT("Extended Stat Mana"));

	static const ConstructorHelpers::FObjectFinder<UCurveFloat> BlockingCurve(TEXT("CurveFloat'/Game/Mirumo/CurveFloats/Blocking_Timeline.Blocking_Timeline'"));
	check(BlockingCurve.Succeeded());
	BlockingFloatCurve = BlockingCurve.Object;

	static const ConstructorHelpers::FObjectFinder<UCurveFloat> ZoomingCurve(TEXT("CurveFloat'/Game/Mirumo/CurveFloats/Zooming_Timeline.Zooming_Timeline'"));
	check(ZoomingCurve.Succeeded());
	ZoomingFloatCurve = ZoomingCurve.Object;

}

// Called to bind functionality to input
void AXYXCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &AXYXCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AXYXCharacter::MoveRight);
		PlayerInputComponent->BindAxis("HorizontalLook", this, &AXYXCharacter::AddControllerYawInput);
		PlayerInputComponent->BindAxis("VerticalLook", this, &AXYXCharacter::AddControllerPitchInput);

		PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AXYXCharacter::LightAttack);
		PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &AXYXCharacter::HeavyAttack);
		PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &AXYXCharacter::SpecialAttack);
		PlayerInputComponent->BindAction("ThrustAttack", IE_Pressed, this, &AXYXCharacter::ThrustAttack);
		PlayerInputComponent->BindAction("Parry", IE_Pressed, this, &AXYXCharacter::ParryAction);
		PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AXYXCharacter::RollAction);
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AXYXCharacter::JumpAction);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &AXYXCharacter::StopJumpAction);
		PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AXYXCharacter::SprintAction);
		PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AXYXCharacter::StopSprintAction);
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AXYXCharacter::CrouchAction);
		PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AXYXCharacter::StopCrouchAction);
		PlayerInputComponent->BindAction("ToggleMovement", IE_Pressed, this, &AXYXCharacter::ToggleMovementAction);
		PlayerInputComponent->BindAction("SwitchMainHandTypeUp", IE_Pressed, this, &AXYXCharacter::SwitchMainHandTypeUpAction);
		PlayerInputComponent->BindAction("SwitchMainHandTypeDown", IE_Pressed, this, &AXYXCharacter::SwitchMainHandTypeDownAction);
		PlayerInputComponent->BindAction("SwitchMainHandItemUp", IE_Pressed, this, &AXYXCharacter::SwitchMainHandItemUpAction);
		PlayerInputComponent->BindAction("SwitchMainHandItemDown", IE_Pressed, this, &AXYXCharacter::SwitchMainHandItemDownAction);
		PlayerInputComponent->BindAction("ToggleCombat", IE_Pressed, this, &AXYXCharacter::ToggleCombatAction);
		PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AXYXCharacter::BlockAction);
		PlayerInputComponent->BindAction("Block", IE_Released, this, &AXYXCharacter::StopBlockAction);
		PlayerInputComponent->BindAction("SwitchSomething", IE_Pressed, this, &AXYXCharacter::SwitchSomethingAction);
		PlayerInputComponent->BindAction("BowAttack", IE_Pressed, this, &AXYXCharacter::StartBowAimModeAttackAction);
		PlayerInputComponent->BindAction("BowAttack", IE_Released, this, &AXYXCharacter::EndBowAimModeAttackAction);
		PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AXYXCharacter::StartZooming);
		PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AXYXCharacter::StopZooming);
		PlayerInputComponent->BindAction("SlowMotion", IE_Pressed, this, &AXYXCharacter::StartSlowMotion);
		PlayerInputComponent->BindAction("SlowMotion", IE_Released, this, &AXYXCharacter::StopSlowMotion);

		PlayerInputComponent->BindAction("TargetLeft", IE_Released, this, &AXYXCharacter::TargetLeftAction);
		PlayerInputComponent->BindAction("TargetRight", IE_Released, this, &AXYXCharacter::TargetRightAction);
		PlayerInputComponent->BindAction("ToggleTargeting", IE_Released, this, &AXYXCharacter::ToggleTargetAction);
	}
}

void AXYXCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PrevCustomMode);
	UpdateRotationSettings();
	UpdateZooming();
	StopBowDrawSound();
	/*const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMovementMode"), true);
	UE_LOG(LogTemp, Log, TEXT("Previous movement mode is %s"), *EnumPtr->GetNameStringByValue((int64)PrevMovementMode));*/

	/*const UEnum* EnumPtrZwa = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMovementMode"), true);
	UE_LOG(LogTemp, Log, TEXT("Current movement mode is %s"), *EnumPtrZwa->GetNameStringByValue((int64)GetCharacterMovement()->MovementMode.GetValue()));*/

	if (GetCharacterMovement()->MovementMode.GetValue() == EMovementMode::MOVE_Walking)
	{
		AttemptPlayBowDrawSound();
	}
}

// Called when the game starts or when spawned
void AXYXCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitialzeCharacter();
	RegisterBlockingTimeline();
	RegisterZoomingTimeline();
}


void AXYXCharacter::InitialzeCharacter()
{

	if (DynamicTargetingComp)
	{
		DynamicTargetingComp->Initialize(TargetArrow);
		DynamicTargetingComp->OnTargetingToggled.AddDynamic(this, &AXYXCharacter::HandleOnTargetingToggled);
	}

	if (StatsManagerComp)
	{
		StatsManagerComp->Initialize();
	}

	if (EquipmentComp)
	{
		EquipmentComp->Initialize();
		EquipmentComp->OnInCombatChanged.AddDynamic(this, &AXYXCharacter::HandleOnInCombatChanged);
		EquipmentComp->OnActiveItemChanged.AddDynamic(this, &AXYXCharacter::HandleOnActiveItemChanged);
		EquipmentComp->OnMainHandTypeChanged.AddDynamic(this, &AXYXCharacter::HandleOnMainHandTypeChanged);
		EquipmentComp->OnCombatTypeChanged.AddDynamic(this, &AXYXCharacter::HandleOnCombatTypeChanged);
	}

	if (InputBufferComp)
	{
		InputBufferComp->OnInputBufferConsumed.AddDynamic(this, &AXYXCharacter::HandleInputBufferConsumed);
		InputBufferComp->OnInputBufferClose.AddDynamic(this, &AXYXCharacter::HandleInputBufferClose);
	}

	if (MovementSpeedComp)
	{
		MovementSpeedComp->OnMovementStateStart.AddDynamic(this, &AXYXCharacter::HandleMovementStateStart);
		MovementSpeedComp->OnMovementStateEnd.AddDynamic(this, &AXYXCharacter::HandleMovementStateEnd);
	}

	if (DynamicTargetingComp && TargetArrow)
	{
		DynamicTargetingComp->Initialize(TargetArrow);
	}

	if (StateManagerComp)
	{
		StateManagerComp->OnActivityChanged.AddDynamic(this, &AXYXCharacter::HandleOnActivityChanged);
		StateManagerComp->OnStateChanged.AddDynamic(this, &AXYXCharacter::HandleOnStateChanged);
	}

	if (RotatingComp)
	{
		RotatingComp->OnRotatingEnd.AddDynamic(this, &AXYXCharacter::HandleOnRotatingEnd);
	}

	if (CollisionHandlerComp)
	{
		CollisionHandlerComp->OnHit.AddDynamic(this, &AXYXCharacter::HandleOnHit);
		CollisionHandlerComp->OnCollisionActivated.AddDynamic(this, &AXYXCharacter::HandleOnCollisionActivated);
	}

	if (EffectsManagerComp)
	{
		EffectsManagerComp->OnEffectApplied.AddDynamic(this, &AXYXCharacter::HandleOnEffectApplied);
		EffectsManagerComp->OnEffectRemoved.AddDynamic(this, &AXYXCharacter::HandleOnEffectRemoved);
	}

	if (ExtendedStamina)
	{
		ExtendedStamina->OnValueChanged.AddDynamic(this, &AXYXCharacter::HandleExtendedStaminaOnValueChanged);
	}

	if (ExtendedHealth)
	{
		ExtendedHealth->OnValueChanged.AddDynamic(this, &AXYXCharacter::HandleExtendedHealthOnValueChanged);
	}

	WBInGame = CreateWidget<UXYXUserWidgetInGame>(GetWorld(), WBInGameClass);
	WBInGame->AddToViewport(0);

	StartCameraSettings.Rotation = FollowCamera->GetRelativeRotation();
	StartCameraSettings.ArmLength = CameraBoom->TargetArmLength;
	StartCameraSettings.SocketOffset = CameraBoom->SocketOffset;
	StartCameraSettings.CameraLagSpeed = CameraBoom->CameraLagSpeed;

	bInitialized = true;
}


void AXYXCharacter::RegisterBlockingTimeline()
{
	FOnTimelineFloat BlockingOnTimelineCallback;
	FOnTimelineEventStatic BlockingOnTimelineFinishedCallback;

	if (BlockingFloatCurve)
	{
		BlockingTimeline = NewObject<UTimelineComponent>(this, FName("BlockingTimelineAnimation"));
		BlockingTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
		this->BlueprintCreatedComponents.Add(BlockingTimeline); // Add to array so it gets saved
		BlockingTimeline->SetNetAddressable();	// This component has a stable name that can be referenced for replication

		BlockingTimeline->SetPropertySetObject(this); // Set which object the timeline should drive properties on
		BlockingTimeline->SetDirectionPropertyName(FName("BlockingTimelineDirection"));

		BlockingTimeline->SetLooping(false);
		BlockingTimeline->SetTimelineLength(5.0f);
		BlockingTimeline->SetPlayRate(2.0f);
		BlockingTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		BlockingTimeline->SetPlaybackPosition(0.0f, false);

		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		BlockingOnTimelineCallback.BindUFunction(this, FName{ TEXT("BlockingTimelineCallback") });
		BlockingOnTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("BlockingTimelineFinishedCallback") });
		BlockingTimeline->AddInterpFloat(BlockingFloatCurve, BlockingOnTimelineCallback);
		BlockingTimeline->SetTimelineFinishedFunc(BlockingOnTimelineFinishedCallback);

		BlockingTimeline->RegisterComponent();
	}
}

void AXYXCharacter::RegisterZoomingTimeline()
{
	FOnTimelineFloat ZoomingOnTimelineCallback;
	FOnTimelineEventStatic ZoomingOnTimelineFinishedCallback;

	if (ZoomingFloatCurve)
	{
		ZoomingTimeline = NewObject<UTimelineComponent>(this, FName("ZoomingTimelineAnimation"));
		ZoomingTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
		this->BlueprintCreatedComponents.Add(ZoomingTimeline); // Add to array so it gets saved
		ZoomingTimeline->SetNetAddressable();	// This component has a stable name that can be referenced for replication

		ZoomingTimeline->SetPropertySetObject(this); // Set which object the timeline should drive properties on

		ZoomingTimeline->SetLooping(false);
		ZoomingTimeline->SetTimelineLength(1.f);
		ZoomingTimeline->SetPlayRate(2.0f);
		ZoomingTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		ZoomingTimeline->SetPlaybackPosition(0.0f, false);

		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		ZoomingOnTimelineCallback.BindUFunction(this, FName{ TEXT("ZoomingTimelineCallback") });
		ZoomingOnTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("ZoomingTimelineFinishedCallback") });
		ZoomingTimeline->AddInterpFloat(ZoomingFloatCurve, ZoomingOnTimelineCallback);
		ZoomingTimeline->SetTimelineFinishedFunc(ZoomingOnTimelineFinishedCallback);

		ZoomingTimeline->RegisterComponent();
	}

}


bool AXYXCharacter::IsEntityAlive_Implementation()
{
	if (StateManagerComp)
	{
		return StateManagerComp->GetState() == EState::EDead ? false : true;
	}

	return true;
}

FName AXYXCharacter::GetHeadSocket_Implementation()
{
	return TEXT("head");
}

float AXYXCharacter::GetAimAlpha_Implementation()
{
	return AimAlpha;
}

bool AXYXCharacter::DoesHoldBowString_Implementation()
{
	if (!EquipmentComp || !GetMesh() || !GetMesh()->GetAnimInstance() ||
		GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return false;
	}

	if (IsIdleAndNotFalling() && EquipmentComp->GetIsInCombat() &&
		EquipmentComp->GetCombatType() == ECombatType::ERanged)
	{
		if (!EquipmentComp->bActionShootOrAimShoot)
		{
			return true;
		}
	}

	return false;
}

FName AXYXCharacter::GetBowStringSocketName_Implementation()
{
	return TEXT("bow_string");
}

FRotator AXYXCharacter::GetDesiredRotation_Implementation() 
{
	FRotator OutRotator = FRotator::ZeroRotator;
	if (!StateManagerComp || !DynamicTargetingComp || !MontageManagerComp)
	{
		return OutRotator;
	}

	if (StateManagerComp->GetState() == EState::EBackstabbing)
	{
		if (IsValid(BackstabbedActor))
		{
			OutRotator.Roll = this->GetActorRotation().Roll;
			OutRotator.Pitch = this->GetActorRotation().Pitch;
			OutRotator.Yaw = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), BackstabbedActor->GetActorLocation()).Yaw;
			return OutRotator;
		}
	}

	if (DynamicTargetingComp->IsTargetingEnabled())
	{
		bool RotateTowardsSelectedTarget = false;
		if (HasMovementInput())
		{
			if (StateManagerComp->GetState() == EState::ERolling)
			{
				if (MontageManagerComp->GetLastRequestedAction() == EMontageAction::ERollForward)
				{
					OutRotator = UKismetMathLibrary::MakeRotFromX(this->GetLastMovementInputVector());
				}
				else
				{
					RotateTowardsSelectedTarget = true;
				}
			}
			else
			{
				RotateTowardsSelectedTarget = true;
			}
		}
		else
		{
			if (StateManagerComp->GetState() == EState::ERolling)
			{
				if (MontageManagerComp->GetLastRequestedAction() == EMontageAction::ERollForward)
				{
					OutRotator = this->GetActorRotation();
				}
				else
				{
					RotateTowardsSelectedTarget = true;
				}
			}
			else
			{
				RotateTowardsSelectedTarget = true;
			}
		}

		if (RotateTowardsSelectedTarget && DynamicTargetingComp->SelectedActor)
		{
			OutRotator.Roll = this->GetActorRotation().Roll;
			OutRotator.Pitch = this->GetActorRotation().Pitch;
			OutRotator.Yaw = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), DynamicTargetingComp->SelectedActor->GetActorLocation()).Yaw;
		}
	}
	else
	{
		if (HasMovementInput())
		{
			OutRotator = UKismetMathLibrary::MakeRotFromX(this->GetLastMovementInputVector());
		}
		else
		{
			OutRotator = this->GetActorRotation();
		}
	}

	return OutRotator;
}

bool AXYXCharacter::TakeAttackDamage_Implementation(FHitData HitData, EAttackResult& ResultType, FVector HitPoint)
{
	UWorld* World = GetWorld();
	check(World);

	ResultType = EAttackResult::EFailed;
	bool TmpResult = false;
	if (CanBeAttacked())
	{
		UpdateReceivedHitDirection(HitData.HitFromDirection);
		if (HitData.Damage != 0.f)
		{
			// If hit was successfully Parried, apply Parried effect on attacker and don't subtract health
			bool bTmpParried = ReceivedHitDirection == EDirection::EFront && 
				StateManagerComp && StateManagerComp->GetActivityValue(EActivity::ECanParryHit) &&
				HitData.CanBeParried;

			if (bTmpParried)
			{
				auto EffectComp = Cast<UXYXEffectsComponent>(HitData.DamageCauser->GetComponentByClass(UXYXEffectsComponent::StaticClass()));
				if (EffectComp)
				{
					bool bTmpApplied = EffectComp->ApplyEffect(EEffectType::EParried, 1.f, EApplyEffectMethod::EReplace, this);
					if (bTmpApplied)
					{
						UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(World->GetGameInstance());
						UXYXFunctionLibrary::PlayParrySound(GameInstance, this, HitData.DamageCauser, this->GetActorLocation());
					}
				}
				ResultType = EAttackResult::EParried;
				return TmpResult;
			}

			// Check if hit was successfully blocked
			bool bTmpBlocked = ReceivedHitDirection == EDirection::EFront && BlockAlpha >= 1.0f && HitData.CanBeBlocked;

			// Ignore stamina if hit was not blocked
			if (StatsManagerComp)
			{
				StatsManagerComp->TakeDamage(HitData.Damage, !bTmpBlocked);

				// If hit was blockedand character is still alive, call block function
				if (Execute_IsEntityAlive(this) && bTmpBlocked)
				{
					UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(World->GetGameInstance());
					UXYXFunctionLibrary::PlayBlockSound(GameInstance, this, HitData.DamageCauser, this->GetActorLocation());

					if (ImpactSparksPS && EquipmentComp && EquipmentComp->GetCombatType() == ECombatType::EMelee)
					{
						UGameplayStatics::SpawnEmitterAtLocation(this, ImpactSparksPS, HitPoint, FRotator::ZeroRotator, FVector::OneVector, true);
					}

					Block();

					// If there is still some stamina left after blocked hit, try to apply Impact effect on attacker
					if (ExtendedStamina && ExtendedStamina->GetCurrentValue() > 0.f)
					{
						// Don't apply if hit was blocked with hands
						if (EquipmentComp && 
							(EquipmentComp->IsShieldEquipped() || !(EquipmentComp->GetCombatType() == ECombatType::EUnarmed)))
						{
							auto EffectComp = Cast<UXYXEffectsComponent>(HitData.DamageCauser->GetComponentByClass(UXYXEffectsComponent::StaticClass()));
							if (EffectComp)
							{
								bool bTmpApplied = EffectComp->ApplyEffect(EEffectType::EImpact, 1.f, EApplyEffectMethod::EReplace, this);
							}
						}
					}
					
					ResultType = EAttackResult::EBlocked;
					return TmpResult;
				}
			}

			if (ImpactBloodPS)
			{
				HitPoint += HitData.HitFromDirection * 21.f;
				UGameplayStatics::SpawnEmitterAtLocation(this, ImpactBloodPS, HitPoint, FRotator::ZeroRotator, FVector::OneVector, true);
			}

			ResultType = EAttackResult::ESuccess;
			TmpResult = true;
		}
	}

	return TmpResult;
}

bool AXYXCharacter::CanEffectBeApplied_Implementation(EEffectType Type, AActor* Applier)
{
	if (CanBeAttacked())
	{
		switch (Type)
		{
		case EEffectType::EStun:
		{
			return CanBeStunned() && CanBeInterrupted();
		}
		break;
		case EEffectType::EBurning:
		{
			return true;
		}
		break;
		case EEffectType::EBackstab:
		{
			return CanBeInterrupted() && CanBeStunned();
		}
		break;
		case EEffectType::EImpact:
		{
			return CanBeInterrupted();
		}
		break;
		case EEffectType::EParried:
		{
			return CanBeInterrupted();
		}
		break;
		}
	}

	return false;
}

void AXYXCharacter::HandleInputBufferConsumed(const EInputBufferKey key)
{
	if (!Execute_IsEntityAlive(this))
		return;

	switch(key)
	{
	case EInputBufferKey::ELightAttack:
		CombatAttack(EMeleeAttackType::ELight);
		break;
	case EInputBufferKey::EHeavyAttack:
		CombatAttack(EMeleeAttackType::EHeavy);
		break;
	case EInputBufferKey::ESpecialAttack:
		CombatAttack(EMeleeAttackType::ESpecial);
		break;
	case EInputBufferKey::EFallingAttack: 
		CombatAttack(EMeleeAttackType::EFalling);
		break;
	case EInputBufferKey::EThrustAttack:
		CombatAttack(EMeleeAttackType::EThrust);
		break;
	case EInputBufferKey::ERoll:
		Roll();
		break;
	case EInputBufferKey::EJump:
		CustomJump();
		break;
	case EInputBufferKey::EParry:
		Parry();
		break;
	case EInputBufferKey::ESwitchMainHandTypeUp:
		SwitchMainHandType(true);
		break;
	case EInputBufferKey::ESwitchMainHandTypeDown:
		SwitchMainHandType(false);
		break;
	case EInputBufferKey::ESwitchMainHandItemUp:
		SwitchMainHandItem(true);
		break;
	case EInputBufferKey::ESwitchMainHandItemDown:
		SwitchMainHandItem(false);
		break;
	case EInputBufferKey::EToggleCombat:
		ToggleCombat();
		break;
	}
}

void AXYXCharacter::HandleInputBufferClose()
{
	if (StateManagerComp && StateManagerComp->GetState() != EState::EDisabled)
	{
		StateManagerComp->ResetState(0.f);
	}
}

void AXYXCharacter::HandleMovementStateStart(EMovementState State)
{
	UWorld* World = GetWorld();
	check(World);

	if (State == EMovementState::ESprint)
	{
		World->GetTimerManager().SetTimer(SprintLoopTimer, this, &AXYXCharacter::SprintLoop, 0.016f, true);
	}
	else if(State == EMovementState::ECrouch)
	{
		World->GetTimerManager().SetTimer(CrouchLoopTimer, this, &AXYXCharacter::CrouchLoop, 0.016f, true);
	}
}

void AXYXCharacter::HandleMovementStateEnd(EMovementState State)
{
	UWorld* World = GetWorld();
	check(World);

	if (State == EMovementState::ESprint)
	{
		World->GetTimerManager().ClearTimer(SprintLoopTimer);
	}
	else if(State == EMovementState::ECrouch)
	{
		World->GetTimerManager().ClearTimer(CrouchLoopTimer);
	}
}

void AXYXCharacter::HandleOnInCombatChanged(bool bIsInCombat)
{
	UpdateRotationSettings();
	if (!bIsInCombat)
	{
		if (StateManagerComp)
		{
			StateManagerComp->SetActivity(EActivity::EIsBlockingPressed, false);
		}

		ResetAimingMode();
	}
}

void AXYXCharacter::HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex)
{
	PlayMainHandTypeChangedMontage(Type);
	if (Type == EItemType::EArrows)
	{
		if (!EquipmentComp->AreArrowEquipped() && EquipmentComp->GetIsInCombat() && 
			EquipmentComp->GetCombatType() == ECombatType::ERanged)
		{
			ResetAimingMode();
		}
	}
}

void AXYXCharacter::HandleOnMainHandTypeChanged(EItemType Type)
{
	PlayMainHandTypeChangedMontage(Type);
}

void AXYXCharacter::HandleOnCombatTypeChanged(ECombatType CombatType)
{
	ResetAimingMode();
}

void AXYXCharacter::HandleOnActivityChanged(EActivity Activity, bool Value)
{
	switch(Activity)
	{
	case EActivity::EIsBlockingPressed:
		UpdateBlocking();
		break;
	case EActivity::EIsLookingForward:
		UpdateRotationSettings();
		break;
	case EActivity::EIsZooming:
		UpdateZooming();
		break;
	}
}

void AXYXCharacter::HandleOnStateChanged(EState PrevState, EState NewState)
{
	UpdateBlocking();
	UpdateZooming();
	UpdateRotationSettings();
	if(PrevState == EState::EAttacking)
	{
		MeleeAttackType = EMeleeAttackType::ENone;
		BackstabbedActor = nullptr;
	}

	StopBowDrawSound();
	if (NewState == EState::EIdle)
	{
		AttemptPlayBowDrawSound();
	}
}

void AXYXCharacter::HandleOnRotatingEnd()
{
	if (StateManagerComp && StateManagerComp->GetState() == EState::EAttacking &&
		(EquipmentComp && EquipmentComp->GetCombatType() == ECombatType::EUnarmed || EquipmentComp->GetCombatType() == ECombatType::EMelee) &&
		EquipmentComp->GetIsInCombat() && DynamicTargetingComp && DynamicTargetingComp->IsTargetingEnabled() && RotatingComp)
	{
		RotatingComp->StartRotatingWithTime(0.5f, 720.f);
	}
}

void AXYXCharacter::HandleOnTargetingToggled(bool bEnabled)
{
	UpdateRotationSettings();
}

void AXYXCharacter::CombatAttack(EMeleeAttackType AttackType)
{
	if (!EquipmentComp)
	{
		return;
	}

	if (EquipmentComp->GetCombatType() == ECombatType::EMelee || EquipmentComp->GetCombatType() == ECombatType::EUnarmed)
	{
		MeleeAttack(AttackType);
	}
	
	if(EquipmentComp->GetCombatType() == ECombatType::ERanged )
	{
		BowActionAttack(AttackType);

		UWorld* World = GetWorld();
		check(World);
		FTimerHandle BowActionAttackTimer;
		World->GetTimerManager().SetTimer(BowActionAttackTimer, this, &AXYXCharacter::PlayBowActionAttackMontage, DelayPlayBowActionAttackMontage, false);
	}
}

void AXYXCharacter::MeleeAttack(EMeleeAttackType AttackType)
{
	if (!CanMeleeAttack())
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		MeleeAttackType = EMeleeAttackType::EFalling;
	}
	else
	{
		MeleeAttackType = AttackType;
	}

	if (StateManagerComp)
		StateManagerComp->SetState(EState::EAttacking);

	UWorld* World = GetWorld();
	check(World);

	World->GetTimerManager().ClearTimer(ResetMeleeAttackCounterTimer);

	UAnimMontage* Montage = GetMeleeAttackMontage(MeleeAttackType);
	if (IsValid(Montage))
	{
		if (GetMesh() && GetMesh()->GetAnimInstance())
		{
			float Duration = -1.f;
			float AttackSpeed = StatsManagerComp ? StatsManagerComp->GetStatValue(EAttributesType::EAttackSpeed, true) : 1.0f;
			Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, AttackSpeed, EMontagePlayReturnType::Duration);
			if (Duration > 0.f)
			{
				Duration *= 0.99f ;
				World->GetTimerManager().SetTimer(ResetMeleeAttackCounterTimer, this, &AXYXCharacter::ResetMeleeAttackCounter, Duration, false);

				if (StatsManagerComp)
				{
					float StaminaCost = StatsManagerComp->GetStatValue(EAttributesType::EMeleeAttackStaminaCost, true);
					StaminaCost =	UXYXFunctionLibrary::ScaleMeleeAttackStaminaCostByType(StaminaCost, MeleeAttackType);
					if (ExtendedStamina)
					{
						ExtendedStamina->ModifyStat(StaminaCost * -1.f, true);
					}
				}
			}
		}
	}
	else
	{
		if (StateManagerComp)
			StateManagerComp->ResetState(0.f);
		ResetMeleeAttackCounter();
	}
}

void AXYXCharacter::BowActionAttack(EMeleeAttackType AttackType)
{
	if (!CanBowAttack() || !EquipmentComp->bActionShootOrAimShoot)
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		MeleeAttackType = EMeleeAttackType::EFalling;
	}
	else
	{
		MeleeAttackType = AttackType;
	}

	StartLookingForward();

	ShowCrosshair(nullptr);
}

void AXYXCharacter::PlayBowActionAttackMontage()
{
	if (StateManagerComp && StateManagerComp->GetState() != EState::EIdle)
	{
		return;
	}

	if (StateManagerComp)
		StateManagerComp->SetState(EState::EAttacking);

	UWorld* World = GetWorld();
	check(World);

	UAnimMontage* Montage = GetMeleeAttackMontage(MeleeAttackType);
	if (IsValid(Montage))
	{
		World->GetTimerManager().ClearTimer(ResetMeleeAttackCounterTimer);
		World->GetTimerManager().ClearTimer(StopLookingForwardTimer);
		World->GetTimerManager().ClearTimer(HideCrosshairTimer);

		if (GetMesh() && GetMesh()->GetAnimInstance())
		{
			float Duration = -1.f;
			float AttackSpeed = 1.0f;
			Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, AttackSpeed, EMontagePlayReturnType::Duration);
			if (Duration > 0.f)
			{
				Duration = Duration * 0.99f;
				World->GetTimerManager().SetTimer(ResetMeleeAttackCounterTimer, this, &AXYXCharacter::ResetMeleeAttackCounter, Duration + DelayPlayBowActionAttackMontage, false);
				World->GetTimerManager().SetTimer(StopLookingForwardTimer, this, &AXYXCharacter::StopLookingForward, Duration + 0.8f, false);
				World->GetTimerManager().SetTimer(HideCrosshairTimer, this, &AXYXCharacter::HideCrosshair, Duration + 0.8f, false);
			}
		}
	}
	else
	{
		if (StateManagerComp)
			StateManagerComp->ResetState(0.f);
		ResetMeleeAttackCounter();
	}
}

bool AXYXCharacter::CanMeleeAttack()
{
	if (StateManagerComp && StateManagerComp->GetState() == EState::EIdle &&
		EquipmentComp && EquipmentComp->GetIsInCombat() &&
		(EquipmentComp->GetCombatType() == ECombatType::EUnarmed || EquipmentComp->GetCombatType()== ECombatType::EMelee ))
	{
		return true;
	}

	return false;
}

bool AXYXCharacter::CanBowAttack()
{
	if (StateManagerComp && StateManagerComp->GetState() == EState::EIdle &&
		EquipmentComp && EquipmentComp->GetIsInCombat() && EquipmentComp->AreArrowEquipped() &&
		EquipmentComp->GetCombatType() == ECombatType::ERanged)
	{
		return true;
	}

	return false;
}

UAnimMontage* AXYXCharacter::GetMeleeAttackMontage(const EMeleeAttackType AttackType)
{
	UAnimMontage* Montage = nullptr;
	EMontageAction Action = UXYXFunctionLibrary::ConvertMeleeAttackTypeToAction(AttackType);
	int32 LastIndex = 0;
	if (MontageManagerComp)
	{
		LastIndex = MontageManagerComp->GetMontageActionLastIndex(Action);

		Montage = MontageManagerComp->GetMontageForAction(
			Action, MeleeAttackCounter > LastIndex ? LastIndex : MeleeAttackCounter
		);
	}

	MeleeAttackCounter++;
	if (MeleeAttackCounter > LastIndex)
		MeleeAttackCounter = 0;

	return Montage;
}

UAnimMontage* AXYXCharacter::GetRollMontage()
{
	UAnimMontage* Montage = nullptr;
	EMontageAction RollDirection = EMontageAction::ENone;

	if (!HasMovementInput())
	{
		RollDirection = EMontageAction::ERollBackward;
	}
	else
	{
		RollDirection = EMontageAction::ERollForward;
	}

	if (MontageManagerComp)
		Montage = MontageManagerComp->GetMontageForAction(RollDirection, 0);
	
	return Montage;
}

UAnimMontage* AXYXCharacter::GetStunMontage(EDirection Direction)
{
	UAnimMontage* Montage = nullptr;
	EMontageAction StunDirection = EMontageAction::EStunFront;

	switch (Direction) 
	{
	case EDirection::EFront:
		StunDirection = EMontageAction::EStunFront;
		break;
	case EDirection::EBack:
		StunDirection = EMontageAction::EStunBack;
		break;
	case EDirection::ELeft:
		StunDirection = EMontageAction::EStunLeft;
		break;
	case EDirection::ERight:
		StunDirection = EMontageAction::EStunRight;
		break;
	}

	if (MontageManagerComp)
		Montage = MontageManagerComp->GetMontageForAction(StunDirection, 0);
	
	if (!IsValid(Montage) && StunDirection != EMontageAction::EStunFront)
	{
		if (MontageManagerComp)
			Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EStunFront, 0);
	}

	return Montage;
}

UAnimMontage* AXYXCharacter::GetBlockMontage()
{
	UAnimMontage* Montage = nullptr;

	if (EquipmentComp && MontageManagerComp)
	{
		int32 Index = EquipmentComp->IsShieldEquipped() ? 1 : 0;
		Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EBlock, Index);
	}

	return Montage;
}

UAnimMontage* AXYXCharacter::GetImpactMontage()
{
	UAnimMontage* Montage = nullptr;

	if (MontageManagerComp)
		Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EImpact, 0);

	return Montage;
}

UAnimMontage* AXYXCharacter::GetParriedMontage()
{
	UAnimMontage* Montage = nullptr;

	if (MontageManagerComp)
		Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EParried, 0);

	return Montage;
}

UAnimMontage* AXYXCharacter::GetParryMontage()
{
	UAnimMontage* Montage = nullptr;

	if (MontageManagerComp && EquipmentComp) 
	{
		int32 Index = EquipmentComp->IsShieldEquipped() ? 1 : 0;
		Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EParry, Index);
	}

	return Montage;
}

void AXYXCharacter::LightAttack()
{
	if (!EquipmentComp && !InputBufferComp)
	{
		return;
	}

	if (EquipmentComp->GetIsInCombat())
	{
		if (EquipmentComp->GetCombatType() == ECombatType::EUnarmed || 
			EquipmentComp->GetCombatType() == ECombatType::EMelee)
		{
			if (!AttemptBackstab())
			{
				InputBufferComp->UpdateKey(EInputBufferKey::ELightAttack);
			}
		}
		else if(EquipmentComp->GetCombatType() == ECombatType::ERanged)
		{
			InputBufferComp->UpdateKey(EInputBufferKey::ELightAttack);
		}
	}
	else
	{
		InputBufferComp->UpdateKey(EInputBufferKey::EToggleCombat);
	}
}

void AXYXCharacter::HeavyAttack()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::EHeavyAttack);
	}
}

void AXYXCharacter::ThrustAttack()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::EThrustAttack);
	}
}

void AXYXCharacter::SpecialAttack()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::ESpecialAttack);
	}
}

void AXYXCharacter::RollAction()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::ERoll);
	}
}

void AXYXCharacter::JumpAction()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::EJump);
	}
}

void AXYXCharacter::StopJumpAction()
{
	StopJumping();
}

void AXYXCharacter::ParryAction()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::EParry);
	}
}

void AXYXCharacter::SprintAction()
{
	if (!WeaponCanSprint())
		return;

	SetSprintOrCrouch(true, EMovementState::ESprint);
}

void AXYXCharacter::StopSprintAction()
{
	SetSprintOrCrouch(false, EMovementState::ESprint);
}

bool AXYXCharacter::WeaponCanSprint()
{
	if (!EquipmentComp)
	{
		return false;
	}

	EWeaponType CurrentWeaponType = EquipmentComp->GetWeaponType();
	return HasMovementInput() &&
		(	CurrentWeaponType == EWeaponType::ENone ||
			CurrentWeaponType == EWeaponType::EDualSwordRight ||
			CurrentWeaponType == EWeaponType::EOneHandSword ||
			CurrentWeaponType == EWeaponType::EOneHandAndShield ||
			CurrentWeaponType == EWeaponType::EOneHandSwordShield ||
			CurrentWeaponType == EWeaponType::EBow ||
			CurrentWeaponType == EWeaponType::ETwinBlade ||
			CurrentWeaponType == EWeaponType::ETwinDaggerRight ||
			CurrentWeaponType == EWeaponType::ESpell
		);
}

void AXYXCharacter::CrouchAction()
{
	if (!WeaponCanCrouch())
		return;

	SetSprintOrCrouch(true, EMovementState::ECrouch);
}

void AXYXCharacter::StopCrouchAction()
{
	SetSprintOrCrouch(false, EMovementState::ECrouch);
}

void AXYXCharacter::ToggleMovementAction()
{
	if (MovementSpeedComp)
	{
		MovementSpeedComp->ToggleState();
	}
}

bool AXYXCharacter::WeaponCanCrouch()
{
	if (!EquipmentComp)
	{
		return false;
	}

	EWeaponType CurrentWeaponType = EquipmentComp->GetWeaponType();
	return HasMovementInput() &&
		(CurrentWeaponType == EWeaponType::ENone ||
			CurrentWeaponType == EWeaponType::EDualSwordRight ||
			CurrentWeaponType == EWeaponType::EOneHandSword ||
			CurrentWeaponType == EWeaponType::EOneHandAndShield ||
			CurrentWeaponType == EWeaponType::EOneHandSwordShield ||
			CurrentWeaponType == EWeaponType::EBow ||
			CurrentWeaponType == EWeaponType::ETwinDaggerRight ||
			CurrentWeaponType == EWeaponType::ESpell
		);
}

void AXYXCharacter::Roll()
{
	if (!CanRoll())
	{
		return;
	}

	if (StateManagerComp)
		StateManagerComp->SetState(EState::ERolling);
	
	UAnimMontage* Montage = GetRollMontage();
	if (IsValid(Montage))
	{
		PlayAnimMontage(Montage);
		if (ExtendedStamina)
		{
			ExtendedStamina->ModifyStat(RollStaminaCost * -1.f, true);
		}
	}
	else
	{
		if (StateManagerComp)
			StateManagerComp->ResetState(0.f);
	}
}

bool AXYXCharacter::CanRoll()
{
	return IsIdleAndNotFalling();
}

bool AXYXCharacter::IsIdleAndNotFalling()
{
	if (GetCharacterMovement())
	{
		return !GetCharacterMovement()->IsFalling() && IsStateEqual(EState::EIdle);
	}

	return false;
}

bool AXYXCharacter::IsStateEqual(EState StateToCompareWith)
{
	if (StateManagerComp)
	{
		return	StateManagerComp->GetState() == StateToCompareWith;
	}

	return false;
}

bool AXYXCharacter::HasMovementInput()
{
	if (GetCharacterMovement() )
	{
		return !GetCharacterMovement()->GetLastInputVector().Equals(FVector(0.f, 0.f, 0.f), 0.0001);
	}

	return false;
}

void AXYXCharacter::SetSprintOrCrouch(bool bActivate, EMovementState MovementState)
{
	if (bActivate)
	{
		if (MovementSpeedComp)
		{
			StoredMovementState = MovementSpeedComp->GetMovementState();
			if(StoredMovementState == EMovementState::ECrouch ||
				StoredMovementState == EMovementState::ESprint)
			{
				StoredMovementState = EMovementState::EJog;
			}
			MovementSpeedComp->SetMovementState(MovementState);
		}
	}
	else
	{
		UWorld* World = GetWorld();
		check(World);

		if (World->GetTimerManager().IsTimerActive(SprintLoopTimer) ||
			World->GetTimerManager().IsTimerActive(CrouchLoopTimer)) {
			if (MovementSpeedComp)
			{
				MovementSpeedComp->SetMovementState(StoredMovementState);
			}
		}
	}
}

void AXYXCharacter::SprintLoop()
{
	if (MovementSpeedComp && ExtendedStamina && StateManagerComp &&
		MovementSpeedComp->GetMovementState() == EMovementState::ESprint &&
		ExtendedStamina->GetCurrentValue() >= SprintStaminaCost &&
		HasMovementInput() &&
		!StateManagerComp->GetActivityValue(EActivity::EIsLookingForward))
	{
		if (IsIdleAndNotFalling() && UKismetMathLibrary::VSize(this->GetVelocity()) > 10.f)
		{
			if (ExtendedStamina)
			{
				ExtendedStamina->ModifyStat(SprintStaminaCost * -1.0f, true);
			}
		}
	}
	else
	{
		SetSprintOrCrouch(false, EMovementState::ESprint);
	}
}

void AXYXCharacter::CrouchLoop()
{
	if (MovementSpeedComp && ExtendedStamina && StateManagerComp &&
		MovementSpeedComp->GetMovementState() == EMovementState::ECrouch &&
		ExtendedStamina->GetCurrentValue() >= CrouchStaminaCost &&
		!StateManagerComp->GetActivityValue(EActivity::EIsLookingForward))
	{
		if (IsIdleAndNotFalling() && UKismetMathLibrary::VSize(this->GetVelocity()) > 2.f)
		{
			if (ExtendedStamina)
			{
				ExtendedStamina->ModifyStat(CrouchStaminaCost * -1.0f, true);
			}
		}
	}
	else
	{
		SetSprintOrCrouch(false, EMovementState::ECrouch);
	}
}

bool AXYXCharacter::CanUseOrSwitchItem()
{
	if (Execute_IsEntityAlive(this) && IsStateEqual(EState::EIdle))
	{
		return true;
	}

	return false;
}

void AXYXCharacter::SwitchMainHandTypeUpAction()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::ESwitchMainHandTypeUp);
	}
}

void AXYXCharacter::SwitchMainHandTypeDownAction()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::ESwitchMainHandTypeDown);
	}
}

void AXYXCharacter::SwitchMainHandType(bool bForward)
{
	if (CanUseOrSwitchItem() && EquipmentComp)
	{
		EquipmentComp->SwitchMainHandType(bForward);
	}
}

void AXYXCharacter::SwitchMainHandItemUpAction()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::ESwitchMainHandItemUp);
	}
}

void AXYXCharacter::SwitchMainHandItemDownAction()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::ESwitchMainHandItemDown);
	}
}

void AXYXCharacter::SwitchMainHandItem(bool bForward)
{
	if (CanUseOrSwitchItem() && EquipmentComp)
	{
		EItemType Type = EquipmentComp->GetSelectedMainHandType();
		int32 SlotIndex = EquipmentComp->GetSelectedMainHandSlotIndex();
		EquipmentComp->SwitchSlotActiveIndex(Type, SlotIndex, bForward, true);
	}
}

void AXYXCharacter::ToggleCombatAction()
{
	if (StateManagerComp && StateManagerComp->GetActivityValue(EActivity::EIsAmingPressed) &&
		IsIdleAndNotFalling())
	{
		ResetAimingMode();
		StopBowDrawSound();
	}
	else
	{
		if (InputBufferComp)
		{
			InputBufferComp->UpdateKey(EInputBufferKey::EToggleCombat);
		}
	}

}

void AXYXCharacter::ToggleCombat()
{
	if (!IsStateEqual(EState::EIdle))
	{
		return;
	}

	if (StateManagerComp)
	{
		StateManagerComp->SetState(EState::EInteracting);
	}

	if (EquipmentComp && MontageManagerComp)
	{
		UAnimMontage* Montage = nullptr;
		if (EquipmentComp->GetIsInCombat())
		{
			Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EDisarmWeapon, 0);
		}
		else
		{
			Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EDrawWeapon, 0);
		}
		if (IsValid(Montage))
		{
			PlayAnimMontage(Montage);
		}
		else
		{
			EquipmentComp->ToggleCombat();
			StateManagerComp->ResetState(0.f);
		}
	}
}

void AXYXCharacter::PlayMainHandTypeChangedMontage(EItemType Type)
{
	if (EquipmentComp && StateManagerComp && MontageManagerComp)
	{
		if (EquipmentComp->GetSelectedMainHandType() == Type && EquipmentComp->GetIsInCombat())
		{
			StateManagerComp->SetState(EState::EInteracting);
			UAnimMontage* Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EDrawWeapon, 1);
			if (IsValid(Montage))
			{
				PlayAnimMontage(Montage);
			}
		}
		else
		{
			if (!EquipmentComp->GetIsInCombat())
			{
				EquipmentComp->DelayUpdateDisplayedItem();
			}
		}
		StateManagerComp->ResetState(0.1f);
	}
}

void AXYXCharacter::BlockAction()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetActivity(EActivity::EIsBlockingPressed, true);
	}
}

void AXYXCharacter::StopBlockAction()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetActivity(EActivity::EIsBlockingPressed, false);
	}
}

void AXYXCharacter::UpdateBlocking()
{
	bool bPlay = false;
	if (StateManagerComp && EquipmentComp && 
		StateManagerComp->GetActivityValue(EActivity::EIsBlockingPressed) &&
		IsIdleAndNotFalling() && 
		EquipmentComp->CanBlock())
	{
		bPlay = true;
	}

	PlayBlockingTimeline(bPlay);
}

void AXYXCharacter::BlockingTimelineCallback(float Val)
{
	BlockAlpha = BlockingTimeline->GetPlaybackPosition();
}

void AXYXCharacter::BlockingTimelineFinishedCallback()
{
	switch(BlockingTimelineDirection)
	{
	case ETimelineDirection::Type::Forward: 
		{
			if (ExtendedStamina)
			{
				ExtendedStamina->ChangeRegenPercent(25);
			}
		}
		break;
	case ETimelineDirection::Type::Backward:
		{
			if (ExtendedStamina)
			{
				ExtendedStamina->ChangeRegenPercent(100);
			}
		}
		break;
	}
}

void AXYXCharacter::PlayBlockingTimeline(bool bInPlay)
{
	if (BlockingTimeline != NULL)
	{
		if (bInPlay)
		{
			BlockingTimeline->Play();
		}
		else
		{
			BlockingTimeline->Reverse();
		}
	}
}

void AXYXCharacter::SwitchSomethingAction()
{
	if (EquipmentComp && EquipmentComp->GetWeaponType() == EWeaponType::EBow)
	{
		EquipmentComp->ChangeShootMode();
	}
}

void AXYXCharacter::StartBowAimModeAttackAction()
{
	if (!EquipmentComp || !InputBufferComp)
	{
		return;
	}

	if (EquipmentComp->GetIsInCombat())
	{
		if (CanBowAttack())
		{
			if (!EquipmentComp->bActionShootOrAimShoot)
			{
				StartAiming();
				StartLookingForward();
				UpdateZooming();
				ShowCrosshair(nullptr);
				AttemptPlayBowDrawSound();
			}
		}
	}
	else
	{
		InputBufferComp->UpdateKey(EInputBufferKey::EToggleCombat);
	}
}

void AXYXCharacter::EndBowAimModeAttackAction()
{
	if (CanBowAttack())
	{
		UWorld* World = GetWorld();
		check(World);

		if (!EquipmentComp->bActionShootOrAimShoot)
		{
			StopAiming();
			if (AimAlpha >= 0.8)
			{
				ShootArrow();
			}

			World->GetTimerManager().SetTimer(StopLookingForwardTimer, this, &AXYXCharacter::StopLookingForward, 0.8f, false);
			World->GetTimerManager().SetTimer(HideCrosshairTimer, this, &AXYXCharacter::HideCrosshair, 0.8f, false);
			FTimerHandle TmpTimer;
			World->GetTimerManager().SetTimer(TmpTimer, this, &AXYXCharacter::UpdateZooming, 0.81f, false);
			StopBowDrawSound();
		}
	}
}

void AXYXCharacter::ShootArrow()
{
	if (CanBowAttack() && EquipmentComp )
	{
		Execute_ShootArrowProjectile(this);
		if (StateManagerComp && MontageManagerComp)
		{
			StateManagerComp->SetState(EState::EAttacking);
			//Choose montage at index 0 (shoot and draw new arrow) if there are still arrows or at index 1 (only shoot arrow) if there are not 
			int32 Index = EquipmentComp->AreArrowEquipped() ? 0 : 1;
			auto Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EShootArrow, Index);
			if (IsValid(Montage))
			{
				PlayAnimMontage(Montage);
			}
			else
			{
				StateManagerComp->ResetState(0.f);
			}
		}
	}
}

void AXYXCharacter::ShootArrowProjectile_Implementation()
{
	if (EquipmentComp)
	{
		auto TmpItem = EquipmentComp->GetActiveItem(EItemType::EArrows, 0);
		TSubclassOf<class UXYXItemBase> ItemArrowClass = TmpItem.ItemClass;
		if (UKismetSystemLibrary::IsValidClass(ItemArrowClass))
		{
			UXYXItemArrow* ItemArrow = NewObject<UXYXItemArrow>(this, ItemArrowClass);
			if (UKismetSystemLibrary::IsValidClass(ItemArrow->GetProjectile()))
			{
				FTransform TmpTransform = GetSpawnedArrowTranform();
				UWorld* const World = GetWorld();
				check(World);
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				ActorSpawnParams.Owner = this;
				ActorSpawnParams.Instigator = this;
				AXYXArrowProjectileBase* XYXActor = World->SpawnActor<AXYXArrowProjectileBase>(ItemArrow->GetProjectile(), TmpTransform, ActorSpawnParams);
				if (XYXActor && StatsManagerComp)
				{
					float TmpValue = EquipmentComp->bActionShootOrAimShoot ? 1 : AimAlpha;
					XYXActor->Damage = TmpValue * StatsManagerComp->GetDamage();
					XYXActor->InitialSpeed = TmpValue * 7000.0f;
				}

				if (InventoryComp)
				{
					auto Index = InventoryComp->FindIndexById(TmpItem.Id);
					InventoryComp->RemoveItemAtIndex(Index, 1);
				}
			}
		}
	}
}

void AXYXCharacter::AttemptPlayBowDrawSound()
{
	UWorld* World = GetWorld();
	check(World);

	World->GetTimerManager().SetTimer(PlayBowDrawSoundTimer, this, &AXYXCharacter::PlayBowDrawSound, 0.1f, true);
	
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([=]
		{
			World->GetTimerManager().ClearTimer(PlayBowDrawSoundTimer);
		}
	);

	World->GetTimerManager().SetTimer(ClearPlayBowDrawSoundTimer, TimerCallback, 1.f, true);
}

void AXYXCharacter::PlayBowDrawSound()
{
	if (IsIdleAndNotFalling() && CanBowAttack() && StateManagerComp && 
		StateManagerComp->GetActivityValue(EActivity::EIsAmingPressed))
	{
		UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(GetGameInstance());
		if (EffectsAudio && GameInstance)
		{
			USoundBase* TmpSound = nullptr;
			FSoundFX* pRow = GameInstance->SoundDataTable->FindRow<FSoundFX>(TEXT("DrawBow"), "");
			if (pRow)
			{
				TmpSound = pRow->SoundBase;
				EffectsAudio->SetSound(TmpSound);
				EffectsAudio->Play(0.f);
				UWorld* World = GetWorld();
				check(World);
				World->GetTimerManager().ClearTimer(PlayBowDrawSoundTimer);
			}

		}
	}
}

void AXYXCharacter::StopBowDrawSound()
{
	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(PlayBowDrawSoundTimer);
	if (EffectsAudio)
	{
		EffectsAudio->Stop();
	}
}

void AXYXCharacter::StartAiming()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetActivity(EActivity::EIsAmingPressed, true);
	}
}

void AXYXCharacter::StopAiming()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetActivity(EActivity::EIsAmingPressed, false);
	}
}

void AXYXCharacter::StartLookingForward()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetActivity(EActivity::EIsLookingForward, true);
	}

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(StopLookingForwardTimer);
	}
}

void AXYXCharacter::StopLookingForward()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetActivity(EActivity::EIsLookingForward, false);
	}
}

void AXYXCharacter::StartZooming()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetActivity(EActivity::EIsZooming, true);
	}
}

void AXYXCharacter::StopZooming()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetActivity(EActivity::EIsZooming, false);
	}
}

void AXYXCharacter::UpdateRotationSettings()
{
	if (StateManagerComp && StateManagerComp->GetActivityValue(EActivity::EIsLookingForward) && 
		(IsIdleAndNotFalling() || StateManagerComp->GetState() == EState::EAttacking || 
			StateManagerComp->GetState() == EState::EParrying || StateManagerComp->GetState() == EState::EBackstabbing ))
	{
		VerticalLookRate = 25.f;
		HorizontalLookRate = 25.f;
		SetCameraLagSmoothly(ZoomedCameraSettings.CameraLagSpeed);
		if (DynamicTargetingComp && RotatingComp)
		{
			// free camera and face towards camera direction
			DynamicTargetingComp->SetFreeCamera(true);
			RotatingComp->SetRotationMode(ERotationMode::EFaceCamera);
		}
	}
	else
	{
		VerticalLookRate = 45.f;
		HorizontalLookRate = 45.f;
		SetCameraLagSmoothly(StartCameraSettings.CameraLagSpeed);
		if (DynamicTargetingComp && RotatingComp && DynamicTargetingComp->IsTargetingEnabled())
		{
			if (EquipmentComp->GetIsInCombat())
			{
				// set free camera and rotation mode (only if not in roll) based on combat type
				switch(EquipmentComp->GetCombatType())
				{
					case ECombatType::EUnarmed:
					case ECombatType::EMelee:
						DynamicTargetingComp->SetFreeCamera(false);
						RotatingComp->SetRotationMode(ERotationMode::EFaceCamera);
						break;
					case ECombatType::EMagic:
					case ECombatType::ERanged:
						DynamicTargetingComp->SetFreeCamera(true);
						RotatingComp->SetRotationMode(ERotationMode::EOrientToMovement);
						break;
				}
			}
			else
			{
				// free camera and update rotation mode only if state is not equal roll (because roll should use OrientToMovement)
				DynamicTargetingComp->SetFreeCamera(false);
				RotatingComp->SetRotationMode(ERotationMode::EFaceCamera);
			}
		}
		else
		{
			if (RotatingComp)
			{
				// orient to movement (character rotates towards movement input)
				RotatingComp->SetRotationMode(ERotationMode::EOrientToMovement);
			}
		}
	}
}

void AXYXCharacter::UpdateZooming()
{
	bool bPlay = false;
	if (StateManagerComp && StateManagerComp->GetActivityValue(EActivity::EIsLookingForward) &&
		(StateManagerComp->GetState() == EState::EAttacking || IsIdleAndNotFalling()) &&
		StateManagerComp->GetActivityValue(EActivity::EIsZooming) && EquipmentComp && !EquipmentComp->bActionShootOrAimShoot)
	{
		bPlay = true;
	}

	PlayZoomingTimeline(bPlay);
}


void AXYXCharacter::ZoomingTimelineCallback(float Val)
{
	ZoomAlpha = ZoomingTimeline->GetPlaybackPosition();
	FollowCamera->SetRelativeRotation(UKismetMathLibrary::RLerp(StartCameraSettings.Rotation, ZoomedCameraSettings.Rotation, ZoomAlpha, false));
	CameraBoom->TargetArmLength = UKismetMathLibrary::Lerp(StartCameraSettings.ArmLength, ZoomedCameraSettings.ArmLength, ZoomAlpha);
	CameraBoom->SocketOffset = UKismetMathLibrary::VLerp(StartCameraSettings.SocketOffset, ZoomedCameraSettings.SocketOffset, ZoomAlpha);
}

void AXYXCharacter::ZoomingTimelineFinishedCallback()
{

}

void AXYXCharacter::PlayZoomingTimeline(bool bInPlay)
{
	if (ZoomingTimeline != NULL)
	{
		if (bInPlay)
		{
			ZoomingTimeline->Play();
		}
		else
		{
			ZoomingTimeline->Reverse();
		}
	}
}

void AXYXCharacter::UpdateAimAlpha()
{
	UWorld* const World = GetWorld();
	if (World && EquipmentComp && !EquipmentComp->bActionShootOrAimShoot && StateManagerComp)
	{
		float AttackSpeed = StatsManagerComp ? StatsManagerComp->GetStatValue(EAttributesType::EAttackSpeed, true) : 1.0f;

		float To0 = UKismetMathLibrary::FInterpTo_Constant(AimAlpha, 0.f, World->GetDeltaSeconds(), 2.0f);
		float To1 = UKismetMathLibrary::FInterpTo_Constant(AimAlpha, 1.f, World->GetDeltaSeconds(), AttackSpeed);
		bool TmpCondition = StateManagerComp->GetActivityValue(EActivity::EIsAmingPressed) && IsIdleAndNotFalling();
		AimAlpha = TmpCondition ? To1 : To0;
	}
}

FTransform AXYXCharacter::GetSpawnedArrowTranform()
{
	FVector TmpArrowSpawnLocation;
	FVector TmpCameraDirection;
	FRotator TmpArrowSpawnDirection;
	if (ArrowSpawnLocation)
	{
		TmpArrowSpawnLocation = ArrowSpawnLocation->GetComponentLocation();
	}

	if (GetController())
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, 0.f);
		const FVector Direction = YawRotation.Vector();

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Warning! %s "), *GetActorForwardVector().ToString()));

		FVector TmpTo = Direction * UXYXFunctionLibrary::GetCrosshairDistanceLocation() + TmpArrowSpawnLocation;
		FVector TmpFrom = FollowCamera->GetComponentLocation();
		TmpCameraDirection = UKismetMathLibrary::GetDirectionUnitVector(TmpFrom, TmpTo);
		TmpArrowSpawnDirection = UKismetMathLibrary::MakeRotFromX(TmpCameraDirection);
		FVector CameraBoomLocation = CameraBoom->GetComponentLocation();
		FVector FllowCameraLocation = FollowCamera->GetComponentLocation();
		float Length = UKismetMathLibrary::VSize(CameraBoomLocation - FllowCameraLocation);
		FVector TmpLineTraceStart = FllowCameraLocation + TmpCameraDirection * Length;
		FVector TmpLineTraceEnd = FllowCameraLocation + TmpCameraDirection * 10000.f;
		FVector TmpCurTraceDirection;
		for (int32 i = 0; i < 20; ++i) 
		{
			FVector TmpFVec = FVector(0.f, 0.f, i * 5.0f);
			FVector TmpLineTo = TmpLineTraceEnd - TmpFVec;
			FHitResult HitResult;
			TArray<AActor*> actorsToIgnore;
			TmpCurTraceDirection = UKismetMathLibrary::GetDirectionUnitVector(TmpLineTraceStart, TmpLineTo);
			if (UKismetSystemLibrary::LineTraceSingle(this, TmpLineTraceStart - TmpFVec, TmpLineTraceEnd - TmpFVec,
				UEngineTypes::ConvertToTraceType(ECC_XYXProjectileChannel), false, actorsToIgnore, EDrawDebugTrace::None, HitResult, true,
				FLinearColor::Red, FLinearColor::Green, 10.0f))
			{
				FVector TmpImpactPoint = HitResult.ImpactPoint;
				bool CanGoNext = false;
				if (i == 0)
				{
					CanGoNext = true;
					float LengthHitPointFromCamera = UKismetMathLibrary::VSize(FllowCameraLocation - TmpImpactPoint);
					float CosOfAngle = UKismetMathLibrary::Cos(
						UKismetMathLibrary::Acos(
							UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Normal(TmpCameraDirection, 0.0001f), UKismetMathLibrary::Normal(TmpCurTraceDirection, 0.0001f))
						)
					);
					float LengthToPointOnCameraDirectionLineAboveImpactPoint = LengthHitPointFromCamera / CosOfAngle;
					TmpArrowSpawnDirection = UKismetMathLibrary::MakeRotFromX(
						UKismetMathLibrary::GetDirectionUnitVector(TmpArrowSpawnLocation, TmpCameraDirection * LengthToPointOnCameraDirectionLineAboveImpactPoint + FllowCameraLocation));
				}
				else
				{
					if (UKismetMathLibrary::VSize(TmpArrowSpawnLocation - TmpImpactPoint) > 1000.0f)
					{
						CanGoNext = true;
					}
				}

				if (CanGoNext)
				{
					auto TmpHitActor = HitResult.GetActor();
					if (TmpHitActor != GetOwner())
					{
						IXYXInterfaceEntity* TmpHitObj = Cast<IXYXInterfaceEntity>(TmpHitActor);
						if (TmpHitObj)
						{
							float LengthHitPointFromCamera = UKismetMathLibrary::VSize(FllowCameraLocation - TmpImpactPoint);
							float CosOfAngle = UKismetMathLibrary::Cos(
								UKismetMathLibrary::Acos(
									UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Normal(TmpCameraDirection, 0.0001f), UKismetMathLibrary::Normal(TmpCurTraceDirection, 0.0001f))
								)
							);
							float LengthToPointOnCameraDirectionLineAboveImpactPoint = LengthHitPointFromCamera / CosOfAngle;
							TmpArrowSpawnDirection = UKismetMathLibrary::MakeRotFromX(
								UKismetMathLibrary::GetDirectionUnitVector(TmpArrowSpawnLocation, TmpCameraDirection * LengthToPointOnCameraDirectionLineAboveImpactPoint + FllowCameraLocation));
						
							return FTransform(TmpArrowSpawnDirection, TmpArrowSpawnLocation, FVector(1.f, 1.f, 1.f));
						}
					}
				}
			}
		}
	}

	return FTransform(TmpArrowSpawnDirection, TmpArrowSpawnLocation, FVector(1.f, 1.f, 1.f));
}

void AXYXCharacter::ResetAimingMode()
{
	StopLookingForward();
	StopAiming();
	StopZooming();
	HideCrosshair();
}

void AXYXCharacter::SetCameraLagSmoothly(float InTargetLagSpeed)
{
	TargetLagSpeed = InTargetLagSpeed;
	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().SetTimer(UpdateCameraLagTimer, this, &AXYXCharacter::UpdateCameraLag, 0.016f, true);
}

void AXYXCharacter::UpdateCameraLag()
{
	if (CameraBoom && GetWorld())
	{
		CameraBoom->CameraLagSpeed = UKismetMathLibrary::FInterpTo_Constant(
			CameraBoom->CameraLagSpeed, TargetLagSpeed, GetWorld()->GetDeltaSeconds(), 120.f);
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(CameraBoom->CameraLagSpeed, TargetLagSpeed, 0.1))
		{
			GetWorld()->GetTimerManager().ClearTimer(UpdateCameraLagTimer);
		}
	}
}

void AXYXCharacter::CalculateLeanAmount(float& LeanAmount, float& InterpSpeed)
{
	// Keep leaning if character:
	// *	Is in Jog / Sprint movement state
	// * Is Idle * not falling
	// * IsMoving(Velocity > 10)
	// * LookingForward is disabled

	bool TmpCondition = false;
	if (MovementSpeedComp && StateManagerComp)
	{
			TmpCondition = IsIdleAndNotFalling() &&
			(MovementSpeedComp->GetMovementState() == EMovementState::EJog || MovementSpeedComp->GetMovementState() == EMovementState::ESprint) &&
			UKismetMathLibrary::VSize(GetCharacterMovement()->Velocity) > 10.f &&
			!StateManagerComp->GetActivityValue(EActivity::EIsLookingForward);
	}

	LeanAmount = TmpCondition ? UKismetMathLibrary::Clamp(HorizontalLookValue, -1.f, 1.f) : 0.f;
	InterpSpeed = TmpCondition ? 10.f : 1.f;
}

void AXYXCharacter::ShowCrosshair(UTexture2D* InTexture)
{
	GetWorld()->GetTimerManager().ClearTimer(HideCrosshairTimer);

	if (IsValid(WBInGame))
	{
		WBInGame->ShowCrosshair(InTexture);
	}
}

void AXYXCharacter::HideCrosshair()
{
	if (IsValid(WBInGame))
	{
		WBInGame->HideCrosshair();
	}
}

void AXYXCharacter::UpdateCrosshairPosition()
{
	FVector TmpArrowSpawnLocation;
	if (ArrowSpawnLocation)
	{
		TmpArrowSpawnLocation = ArrowSpawnLocation->GetComponentLocation();
	}

	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, 0.f);
	const FVector Direction = YawRotation.Vector();

	FVector TmpTo = Direction * UXYXFunctionLibrary::GetCrosshairDistanceLocation() + TmpArrowSpawnLocation;

	if (IsValid(WBInGame))
	{
		WBInGame->UpdateCrosshairPosition(TmpTo);
	}
}

void AXYXCharacter::Death()
{
	if (StateManagerComp)
	{
		StateManagerComp->SetState(EState::EDead);

		// Disable camera lock
		if (DynamicTargetingComp)
			DynamicTargetingComp->DisableCameraLock();
		
		// Deactivate collision handler
		if (CollisionHandlerComp)
			CollisionHandlerComp->DeactiveCollision();

		// Remove widget
		if (WBInGame)
			WBInGame->RemoveFromParent();

		// Enable physics on Mesh
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"), true);
		GetMesh()->SetSimulatePhysics(true);

		// Enable Physics on main hand/shield items 
		if (EquipmentComp && EquipmentComp->GetIsInCombat())
		{
			AXYXDisplayedItem* DIOne = nullptr;
			AXYXDisplayedItem* DISecond = nullptr;
			EquipmentComp->GetDisplayedItem(
				EquipmentComp->GetSelectedMainHandType(), EquipmentComp->GetSelectedMainHandSlotIndex(), DIOne, DISecond);
			if (DIOne)
				DIOne->SimulatePhysics();
			if (DISecond)
				DISecond->SimulatePhysics();
		}
	}
}

void AXYXCharacter::HandleOnHit(FHitResult HitResult)
{
	IXYXInterfaceEntity* TmpHitActor = Cast<IXYXInterfaceEntity>(HitResult.Actor);
	if (TmpHitActor)
	{
		EAttackResult ResultType;
		bool CanAttacked =  TmpHitActor->Execute_TakeAttackDamage(HitResult.GetActor(), MakeMeleeHitData(HitResult.GetActor()), ResultType, HitResult.Location);
		ApplyHitImpulseToCharacter(HitResult.GetActor(), HitResult.Normal, 40000.f);
		if (CanAttacked)
		{
			UWorld* World = GetWorld();
			check(World);
			UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(World->GetGameInstance());
			UXYXFunctionLibrary::PlayHitSound(GameInstance, this, HitResult.GetActor(), HitResult.Location);

			auto EffectComp = Cast<UXYXEffectsComponent>(HitResult.Actor->GetComponentByClass(UXYXEffectsComponent::StaticClass()));
			if (EffectComp)
			{
				EffectComp->ApplyEffect(EEffectType::EStun, 2.f, EApplyEffectMethod::EReplace, this);
			}
		}
	}
}

void AXYXCharacter::HandleOnCollisionActivated(ECollisionPart CollisionPart)
{
	switch(CollisionPart)
	{
	case ECollisionPart::EMainHandItem:
		{
			// Set Main Hand Item as new Collision mesh
			if (EquipmentComp)
			{
				AXYXDisplayedItem* DIOne = nullptr;
				AXYXDisplayedItem* DISecond = nullptr;
				EquipmentComp->GetDisplayedItem(
					EquipmentComp->GetSelectedMainHandType(), EquipmentComp->GetSelectedMainHandSlotIndex(), DIOne, DISecond);

				if (CollisionHandlerComp)
				{
					if (DIOne)
					{
						UPrimitiveComponent* StaticMeshComp = DIOne->GetPrimaryComponent();
						CollisionHandlerComp->SetCollisionMesh(StaticMeshComp, StaticMeshComp->GetAllSocketNames());
					}
					if (DISecond)
					{
						UPrimitiveComponent* StaticMeshComp = DISecond->GetPrimaryComponent();
						CollisionHandlerComp->SetDualCollisionMesh(StaticMeshComp, StaticMeshComp->GetAllSocketNames());
					}
				}
			}
		}
		break;
	case ECollisionPart::ELeftHand:
		{
			if (CollisionHandlerComp)
			{
				CollisionHandlerComp->SetCollisionMesh(GetMesh(), LeftHandCollisionSockets);
			}
		}
		break;
	case ECollisionPart::ERightHand:
		{
			if (CollisionHandlerComp)
			{
				CollisionHandlerComp->SetCollisionMesh(GetMesh(), RightHandCollisionSockets);
			}
		}
		break;
	case ECollisionPart::ELeftFoot:
		{
			if (CollisionHandlerComp)
			{
				CollisionHandlerComp->SetCollisionMesh(GetMesh(), LeftFootCollisionSockets);
			}
		}
		break;
	case ECollisionPart::ERightFoot:
		{
			if (CollisionHandlerComp)
			{
				CollisionHandlerComp->SetCollisionMesh(GetMesh(), RightFootCollisionSockets);
			}
		}
		break;
	case ECollisionPart::ESpine:
	{
		if (CollisionHandlerComp)
		{
			CollisionHandlerComp->SetCollisionMesh(GetMesh(), SpineCollisionSockets);
		}
	}
	break;
	}
}

FHitData AXYXCharacter::MakeMeleeHitData(AActor* HitActor)
{
	FHitData TmpData;
	if (StatsManagerComp && HitActor)
	{
		TmpData.Damage = UXYXFunctionLibrary::ScaleMeleeDamageByType(StatsManagerComp->GetDamage(), MeleeAttackType);
		TmpData.DamageCauser = this;
		TmpData.HitFromDirection = UKismetMathLibrary::GetDirectionUnitVector(this->GetActorLocation(), HitActor->GetActorLocation());
		TmpData.CanBeParried = MeleeAttackType == EMeleeAttackType::ELight;
		TmpData.CanBeBlocked = true;
		TmpData.CanReceivedImpact = true;
	}

	return TmpData;
}

void AXYXCharacter::ApplyHitImpulseToCharacter(AActor* HitActor, FVector HitNormal, float ImpulsePower)
{
	auto XYXCharacter = Cast<ACharacter>(HitActor);
	if (XYXCharacter && XYXCharacter->GetMesh() &&
		XYXCharacter->GetMesh()->IsAnySimulatingPhysics())
	{
		XYXCharacter->GetMesh()->AddImpulse(HitNormal * -1.0f * ImpulsePower);
	}
}

bool AXYXCharacter::CanBeAttacked()
{
	if (Execute_IsEntityAlive(this))
	{
		if (StateManagerComp)
		{
			return !StateManagerComp->GetActivityValue(EActivity::EIsImmortal);
		}
	}

	return false;
}

bool AXYXCharacter::CanBeStunned()
{
	if (GetCharacterMovement()->IsFalling() || !EffectsManagerComp ||  EffectsManagerComp->IsEffectApplied(EEffectType::EBackstab))
	{
		return false;
	}

	return UKismetMathLibrary::RandomBoolWithWeight(0.4f);
}

bool AXYXCharacter::CanBeInterrupted()
{
	if (StateManagerComp)
	{
		return  !StateManagerComp->GetActivityValue(EActivity::ECanBeInterrupted);
	}

	return true;
}

void AXYXCharacter::UpdateReceivedHitDirection(FVector HitFromDirection)
{
	ReceivedHitDirection = UXYXFunctionLibrary::GetHitDirection(HitFromDirection, this);
}

void AXYXCharacter::Block()
{
	auto Montage = GetBlockMontage();
	if (Montage)
	{
		auto Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f, EMontagePlayReturnType::Duration);
		ResetMeleeAttackCounter();
	}
}

void AXYXCharacter::HandleOnEffectApplied(EEffectType Type)
{
	switch(Type)
	{
	case EEffectType::EStun:
		{
			Stunned();
		}
		break;
	case EEffectType::EBackstab:
		{
			Backstabbed();
		}
		break;
	case EEffectType::EParried:
		{
			Parried();
		}
		break;
	case EEffectType::EImpact:
		{
			Impact();
		}
		break;
	}

	if (StateManagerComp)
	{
		StateManagerComp->SetState(EState::EDisabled);
		if (InputBufferComp)
		{
			InputBufferComp->UpdateKey(EInputBufferKey::ENone);
			ResetMeleeAttackCounter();
			// Open input buffer 	(it will be closed when effect ends)
			InputBufferComp->OpenInputBuffer();
		}
	}
}

void AXYXCharacter::HandleOnEffectRemoved(EEffectType Type)
{
	switch (Type)
	{
	case EEffectType::EStun:
	case EEffectType::EBackstab:
	case EEffectType::EParried:
	case EEffectType::EImpact:
	{
		if (EffectsManagerComp)
		{
			TArray<EEffectType> Types = { EEffectType::EStun, 
				EEffectType::EKnockdown, EEffectType::EImpact, 
				EEffectType::EParried, EEffectType::EBackstab };

			bool bApplied = EffectsManagerComp->IsAnyEffectApplied(Types);
			if (!bApplied)
			{
				if (StateManagerComp)
				{
					StateManagerComp->ResetState(0.0f);
					// Close input buffer (Perform action based on stored key)
					if (InputBufferComp)
					{
						InputBufferComp->CloseInputBuffer();
					}
				}
			}
		}
	}
	break;
	}
}

void AXYXCharacter::Stunned()
{
	auto Montage = GetStunMontage(ReceivedHitDirection);
	if (Montage)
	{
		auto Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f, EMontagePlayReturnType::Duration);
		if (EffectsManagerComp)
		{
			EffectsManagerComp->AdjustEffectTime(EEffectType::EStun, Duration * 0.8f);
		}
	}
	else
	{
		if (StateManagerComp)
		{
			StateManagerComp->ResetState(0.f);
		}
	}
}

void AXYXCharacter::Backstabbed()
{
	if (MontageManagerComp)
	{
		auto Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EBackstabbed, 0);
		if (Montage)
		{
			auto Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f, EMontagePlayReturnType::Duration);
			if (EffectsManagerComp)
			{
				EffectsManagerComp->AdjustEffectTime(EEffectType::EBackstab, Duration * 0.8f);
			}
		}
		else
		{
			if (StateManagerComp)
			{
				StateManagerComp->ResetState(0.f);
			}
		}
	}	
}

void AXYXCharacter::Impact()
{
	if (MontageManagerComp)
	{
		auto Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EImpact, 0);
		if (Montage)
		{
			auto Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f, EMontagePlayReturnType::Duration);
			if (EffectsManagerComp)
			{
				EffectsManagerComp->AdjustEffectTime(EEffectType::EImpact, Duration * 0.8f);
			}
		}
		else
		{
			if (StateManagerComp)
			{
				StateManagerComp->ResetState(0.f);
			}
		}
	}
}

void AXYXCharacter::Parried()
{
	auto Montage = GetParryMontage();
	if (Montage)
	{
		auto Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f, EMontagePlayReturnType::Duration);
		if (EffectsManagerComp)
		{
			EffectsManagerComp->AdjustEffectTime(EEffectType::EParried, Duration * 0.8f);
		}
	}
	else
	{
		if (StateManagerComp)
		{
			StateManagerComp->ResetState(0.f);
		}
	}
}

bool AXYXCharacter::AttemptBackstab()
{
	if (CanMeleeAttack())
	{
		// Check if there is any avaialable to backstab actor in front of the character
		FVector TmpStartLocation = GetActorLocation();
		FVector TmpEndLocation = GetActorForwardVector() * 150.f + TmpStartLocation;
		FHitResult HitResult;
		TArray<AActor*> actorsToIgnore;
		if (UKismetSystemLibrary::LineTraceSingle(this, TmpStartLocation, TmpEndLocation,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::None, HitResult, true,
			FLinearColor::Red, FLinearColor::Green, 5.f))
		{
			AActor* TmpHitActor = HitResult.GetActor();
			if (TmpHitActor && TmpHitActor->GetDotProductTo(this) < -0.25f)
			{
				// Apply Backstab effect
				auto EffectComp = Cast<UXYXEffectsComponent>(TmpHitActor->GetComponentByClass(UXYXEffectsComponent::StaticClass()));
				if (EffectComp && StatsManagerComp)
				{
					bool bApplied = EffectComp->ApplyBackstabEffect(2.f, EApplyEffectMethod::EReplace, this, StatsManagerComp->GetDamage() * 3.0f);
					if (bApplied)
					{
						BackstabbedActor = TmpHitActor;
						if (StateManagerComp)
						{
							StateManagerComp->SetState(EState::EBackstabbing);

							if (MontageManagerComp)
							{
								auto Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EBackstab, 0);
								if (Montage)
								{
									auto Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f, EMontagePlayReturnType::Duration);
									if (ExtendedStamina)
									{
										// Remove Stamina
										ExtendedStamina->ModifyStat(-40.f, true);
										return true;
									}
								}
								else
								{
									if (StateManagerComp)
									{
										StateManagerComp->ResetState(0.f);
										return false;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return false;
}

float AXYXCharacter::GetCastingSpeed()
{
	if (StatsManagerComp)
	{
		return StatsManagerComp->GetStatValue(EAttributesType::ECastingSpeed, true);
	}

	return 0.f;
}

float AXYXCharacter::GetMagicDamage()
{
	if (StatsManagerComp)
	{
		return StatsManagerComp->GetStatValue(EAttributesType::EMagicDamage, true);
	}

	return 0.f;
}

void AXYXCharacter::StartSlowMotion()
{
	if (CanEnterSlowMotion())
	{
		bIsInSlowMotion = true;
		UGameplayStatics::SetGlobalTimeDilation(this, SlowMotionTimeDilation);
		UWorld* World = GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(LoopSlowMotionTimer, this, &AXYXCharacter::LoopSlowMotion, 0.016f, true);
	}
}

void AXYXCharacter::StopSlowMotion()
{
	bIsInSlowMotion = false; 
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(LoopSlowMotionTimer);
}

void AXYXCharacter::LoopSlowMotion()
{
	if (bIsInSlowMotion && CanEnterSlowMotion() && IsEnoughStamina(SlowMotionStaminaCost))
	{
		if (ExtendedStamina)
		{
			ExtendedStamina->ModifyStat(SlowMotionStaminaCost * -1.f, true);
		}
	}
	else
	{
		StopSlowMotion();
	}
}

bool AXYXCharacter::CanEnterSlowMotion()
{
	return StateManagerComp && StateManagerComp->GetActivityValue(EActivity::EIsLookingForward) && 
		(StateManagerComp->GetState() == EState::EAttacking || IsIdleAndNotFalling());
}

bool AXYXCharacter::IsEnoughStamina(float Value)
{
	if (ExtendedStamina)
	{
		return ExtendedStamina->GetCurrentValue() >= Value;
	}

	return false;
}

void AXYXCharacter::HandleExtendedHealthOnValueChanged(float NewValue, float MaxValue)
{
	if (NewValue <= 0.f)
	{
		Death();
	}
}

void AXYXCharacter::HandleExtendedStaminaOnValueChanged(float NewValue, float MaxValue)
{
	if (NewValue / MaxValue <= 0.1f)
	{
		// Update blocking if stamina drops below 10%
		UpdateBlocking();
	}
}

void AXYXCharacter::TargetLeftAction()
{
	if (DynamicTargetingComp)
	{
		DynamicTargetingComp->FindTargetOnLeft();
	}
}

void AXYXCharacter::TargetRightAction()
{
	if (DynamicTargetingComp)
	{
		DynamicTargetingComp->FindTargetOnRight();
	}
}

void AXYXCharacter::ToggleTargetAction()
{
	if (Execute_IsEntityAlive(this) && DynamicTargetingComp)
	{
		DynamicTargetingComp->ToggleCameraLock();
	}
}

void AXYXCharacter::CustomJump()
{
	if (!CanJump() || !IsStateEqual(EState::EIdle))
	{
		return;
	}

	if (GetMesh() && GetMesh()->GetAnimInstance() 
		&& GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
			GetMesh()->GetAnimInstance()->Montage_Stop(0.25f);
	}

	Jump();
}

void AXYXCharacter::Parry()
{
	if (!GetCharacterMovement() || GetCharacterMovement()->IsFalling() || !CanMeleeAttack())
	{
		return;
	}

	if (StateManagerComp)
		StateManagerComp->SetState(EState::EParrying);

	UAnimMontage* Montage = GetParryMontage();
	if (Montage)
	{
		if (GetMesh() && GetMesh()->GetAnimInstance())
		{
			float AttackSpeed = StatsManagerComp ? StatsManagerComp->GetStatValue(EAttributesType::EAttackSpeed, true) : 1.0f;
			GetMesh()->GetAnimInstance()->Montage_Play(Montage, AttackSpeed, EMontagePlayReturnType::Duration);

			if (ExtendedStamina)
			{
				ExtendedStamina->ModifyStat(-15.f, true);
			}
		}
	}
	else
	{
		if (StateManagerComp)
			StateManagerComp->ResetState(0.f);
	}
}

// Called every frame
void AXYXCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BlockingTimeline != NULL)
	{
		BlockingTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}

	UpdateAimAlpha();
	UpdateCrosshairPosition();
}

void AXYXCharacter::PostInitProperties() {
	Super::PostInitProperties();
}

void AXYXCharacter::PreInitializeComponents() {
	Super::PreInitializeComponents();
}

void AXYXCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

UXYXAnimInstance* AXYXCharacter::GetXYXAnimInstance() const {
	return Cast<UXYXAnimInstance>(GetMesh()->GetAnimInstance());
}

void AXYXCharacter::SetGenericTeamId(const FGenericTeamId& InTeamID) {
	CombatTeam = (ETeam)InTeamID.GetId();
}

FGenericTeamId AXYXCharacter::GetGenericTeamId() const {
	return uint8(CombatTeam);
}

void AXYXCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AXYXCharacter, CombatTeam);
}

void AXYXCharacter::GetMovementVectors(FVector& ForwardVec, FVector& RightVec)
{
	if (GetController())
	{
		const FRotator _Rotation = GetController()->GetControlRotation();
		const FRotator _YawRotation(0, _Rotation.Yaw, 0);
		ForwardVec = FRotationMatrix(_YawRotation).GetUnitAxis(EAxis::X);
		RightVec = FRotationMatrix(_YawRotation).GetScaledAxis(EAxis::Y);
	}
}

UDataTable* AXYXCharacter::GetMontages_Implementation(EMontageAction Action)
{
	UDataTable* DataTable = nullptr;
	TArray<EMontageAction> CommonMontages = {
		EMontageAction::EBackstabbed,
		EMontageAction::ERollForward,
		EMontageAction::ERollBackward,
		EMontageAction::ERollLeft,
		EMontageAction::ERollRight
	};

	UWorld* World = GetWorld();
	check(World);

	UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(World->GetGameInstance());
	if (GameInstance)
	{
		if (CommonMontages.Contains(Action)) 
		{
			if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Common")))
				DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Common")];
		}
		else
		{
			if (!EquipmentComp)
			{
				return nullptr;
			}

			bool SwitchType = false;
			if (EquipmentComp->GetIsInCombat())
			{
				SwitchType = true;
			}
			else
			{
				if (Action == EMontageAction::EDisarmWeapon || Action == EMontageAction::EDrawWeapon)
				{
					SwitchType = true;
				}
				else
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Unarmed")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Unarmed")];
				}
			}

			if (!SwitchType)
			{
				return DataTable;
			}

			switch (EquipmentComp->GetWeaponType())
			{
			case EWeaponType::ENone: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Unarmed")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Unarmed")];
				}
				break;
			case EWeaponType::EBow: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Bow")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Bow")];
				}
				break;
			case EWeaponType::EDualSwordRight: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_DualSword")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_DualSword")];
				}
				break;
			case EWeaponType::EGreatSword: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_GreatSword")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_GreatSword")];
				}
				break;
			case EWeaponType::EOneHandAndShield: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_OneHandAndShield")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_OneHandAndShield")];
				}
				break;
			case EWeaponType::EOneHandSword: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_OneHandSword")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_OneHandSword")];
				}
				break;
			case EWeaponType::EOneHandSwordShield: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_OneHandSwordShield")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_OneHandSwordShield")];
				}
				break;
			case EWeaponType::ESpell: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Spell")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Spell")];
				}
				break;
			case EWeaponType::ETwinBlade: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_TwinBlade")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_TwinBlade")];
				}	
				break;
			case EWeaponType::ETwinDaggerRight: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_TwinDagger")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_TwinDagger")];
				}
				break;
			case EWeaponType::ETwoHandAxe: 
				{
					if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_TwoHandAxe")))
						DataTable = GameInstance->MontageDataTables[TEXT("Heroe_TwoHandAxe")];
				}
				break;
			case EWeaponType::ETwoHandHammer: 
				{
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_TwoHandHammer")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_TwoHandHammer")];
				}
				break;
			}
		}
	}

	return DataTable;
}

void AXYXCharacter::MoveRight(float Val)
{
	if (Val != 0.f && GetController())
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}
}

void AXYXCharacter::MoveForward(float Val)
{
	if (Val != 0.f && GetController())
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
		
	}
}

void AXYXCharacter::AddControllerYawInput(float Val)
{
	UWorld* World = GetWorld();
	check(World);

	if (Val != 0.f)
	{
		HorizontalLookValue = Val;
		Val = Val * HorizontalLookRate * World->GetDeltaSeconds();
		APawn::AddControllerYawInput(Val);

		if (DynamicTargetingComp)
		{
			DynamicTargetingComp->FindTargetWithAxisInput(HorizontalLookValue);
		}
	}
}

void AXYXCharacter::AddControllerPitchInput(float Val)
{
	UWorld* World = GetWorld();
	check(World);

	if (Val != 0.f)
	{
		Val = Val * VerticalLookRate * World->GetDeltaSeconds();
		APawn::AddControllerPitchInput(Val);
	}
}
