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

	static const ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/Game/Mirumo/CurveFloats/Blocking_Timeline.Blocking_Timeline'"));
	check(Curve.Succeeded());
	BlockingFloatCurve = Curve.Object;

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
	}
}

// Called when the game starts or when spawned
void AXYXCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitialzeCharacter();
	RegisterBlockingTimeline();
}

void AXYXCharacter::RegisterBlockingTimeline()
{
	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;

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
		BlockingTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		BlockingTimeline->SetPlaybackPosition(0.0f, false);

		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		onTimelineCallback.BindUFunction(this, FName{ TEXT("BlockingTimelineCallback") });
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("BlockingTimelineFinishedCallback") });
		BlockingTimeline->AddInterpFloat(BlockingFloatCurve, onTimelineCallback);
		BlockingTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		BlockingTimeline->RegisterComponent();
	}
}


bool AXYXCharacter::IsEntityAlive_Implementation()
{
	if (StateManagerComp)
	{
		return StateManagerComp->GetState() == EState::EDead ? false : true;
	}

	return !bIsDead;
}

void AXYXCharacter::InitialzeCharacter()
{
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

	if (EquipmentComp)
	{
		EquipmentComp->Initialize();
		EquipmentComp->OnInCombatChanged.AddDynamic(this, &AXYXCharacter::HandleOnInCombatChanged);
		EquipmentComp->OnActiveItemChanged.AddDynamic(this, &AXYXCharacter::HandleOnActiveItemChanged);
		EquipmentComp->OnMainHandTypeChanged.AddDynamic(this, &AXYXCharacter::HandleOnMainHandTypeChanged);
		EquipmentComp->OnCombatTypeChanged.AddDynamic(this, &AXYXCharacter::HandleOnCombatTypeChanged);
	}

	if (StateManagerComp)
	{
		StateManagerComp->OnActivityChanged.AddDynamic(this, &AXYXCharacter::HandleOnActivityChanged);
		StateManagerComp->OnStateChanged.AddDynamic(this, &AXYXCharacter::HandleOnStateChanged);
	}

	StartCameraSettings.Rotation = FollowCamera->GetRelativeRotation();
	StartCameraSettings.ArmLength = CameraBoom->TargetArmLength;
	StartCameraSettings.SocketOffset = CameraBoom->SocketOffset;
	StartCameraSettings.CameraLagSpeed = CameraBoom->CameraLagSpeed;

	bInitialized = true;
}

void AXYXCharacter::HandleInputBufferConsumed(const EInputBufferKey key)
{
	if (!IsEntityAlive())
		return;

	switch(key)
	{
	case EInputBufferKey::ELightAttack:
		MeleeAttack(EMeleeAttackType::ELight);
		break;
	case EInputBufferKey::EHeavyAttack:
		MeleeAttack(EMeleeAttackType::EHeavy);
		break;
	case EInputBufferKey::ESpecialAttack:
		MeleeAttack(EMeleeAttackType::ESpecial);
		break;
	case EInputBufferKey::EFallingAttack: 
		MeleeAttack(EMeleeAttackType::EFalling);
		break;
	case EInputBufferKey::EThrustAttack:
		MeleeAttack(EMeleeAttackType::EThrust);
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
	if (StateManagerComp)
	{
		StateManagerComp->SetActivity(EActivity::EIsBlockingPressed, false);
	}
}

void AXYXCharacter::HandleOnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int32 SlotIndex, int32 ActiveIndex)
{
	PlayMainHandTypeChangedMontage(Type);
}

void AXYXCharacter::HandleOnMainHandTypeChanged(EItemType Type)
{
	PlayMainHandTypeChangedMontage(Type);
}

void AXYXCharacter::HandleOnCombatTypeChanged(ECombatType CombatType)
{

}

void AXYXCharacter::HandleOnActivityChanged(EActivity Activity, bool Value)
{
	switch(Activity)
	{
	case EActivity::EIsBlockingPressed:
		UpdateBlocking();
		break;
	case EActivity::EIsLookingForward:
		break;
	case EActivity::EIsZooming:
		break;
	}
}

void AXYXCharacter::HandleOnStateChanged(EState PrevState, EState NewState)
{
	UpdateBlocking();
	if(PrevState == EState::EAttacking)
	{
		MeleeAttackType = EMeleeAttackType::ENone;
		BackstabbedActor = nullptr;
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
			float AttackSpeed = 1.0f;
			Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, AttackSpeed, EMontagePlayReturnType::Duration);
			if (Duration > 0.f)
			{
				Duration *= 0.99f;
				World->GetTimerManager().SetTimer(ResetMeleeAttackCounterTimer, this, &AXYXCharacter::ResetMeleeAttackCounter, Duration, false);
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
		(EquipmentComp->GetCombatType() == ECombatType::EUnarmed || EquipmentComp->GetCombatType()== ECombatType::EMelee))
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
		InputBufferComp->UpdateKey(EInputBufferKey::ELightAttack);
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
			CurrentWeaponType == EWeaponType::ESpear ||
			CurrentWeaponType == EWeaponType::EBow ||
			CurrentWeaponType == EWeaponType::ETwinBlade ||
			CurrentWeaponType == EWeaponType::ETwinDaggerRight ||
			CurrentWeaponType == EWeaponType::EKatana ||
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
	if (MovementSpeedComp && 
		MovementSpeedComp->GetMovementState() == EMovementState::ESprint)
	{

	}
	else
	{
		SetSprintOrCrouch(false, EMovementState::ESprint);
	}
}

void AXYXCharacter::CrouchLoop()
{
	if (MovementSpeedComp && 
		MovementSpeedComp->GetMovementState() == EMovementState::ECrouch)
	{

	}
	else
	{
		SetSprintOrCrouch(false, EMovementState::ECrouch);
	}
}

bool AXYXCharacter::CanUseOrSwitchItem()
{
	if (IsEntityAlive() && IsStateEqual(EState::EIdle))
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
		auto&& Weapon = EquipmentComp->GetWeapon();
		if (Weapon.ItemWeaponType == EWeaponType::EDualSwordRight ||
				Weapon.ItemWeaponType == EWeaponType::ETwinDaggerRight)
		{
			EquipmentComp->SwitchSlotActiveIndex(EItemType::EMeleeWeaponLeft, SlotIndex, bForward, true);
		}
	}
}

void AXYXCharacter::ToggleCombatAction()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::EToggleCombat);
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
	
}

void AXYXCharacter::PlayBlockingTimeline(bool bInPlay)
{
	if (BlockingTimeline != NULL)
	{
		if (bInPlay)
		{
			BlockingTimeline->PlayFromStart();
		}
		else
		{
			BlockingTimeline->ReverseFromEnd();
		}
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
			GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.f, EMontagePlayReturnType::Duration);
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

UXYXAnimInstance* AXYXCharacter::GetACFAnimInstance() const {
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
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Unarmed")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Unarmed")];
				break;
			case EWeaponType::EBow:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Bow")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Bow")];
				break;
			case EWeaponType::EDualSwordRight:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_DualSword")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_DualSword")];
				break;
			case EWeaponType::EGreatSword:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_GreatSword")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_GreatSword")];
				break;
			case EWeaponType::EKatana:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Katana")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Katana")];
				break;
			case EWeaponType::EOneHandAndShield:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_OneHandAndShield")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_OneHandAndShield")];
				break;
			case EWeaponType::EOneHandSword:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_OneHandSword")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_OneHandSword")];
				break;
			case EWeaponType::EOneHandSwordShield:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_OneHandSwordShield")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_OneHandSwordShield")];
				break;
			case EWeaponType::ESpear:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Spear")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Spear")];
				break;
			case EWeaponType::ESpell:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Spell")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Spell")];
				break;
			case EWeaponType::ETwinBlade:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_TwinBlade")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_TwinBlade")];
				break;
			case EWeaponType::ETwinDaggerRight:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_TwinDagger")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_TwinDagger")];
				break;
			case EWeaponType::ETwoHandAxe:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_TwoHandAxe")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_TwoHandAxe")];
				break;
			case EWeaponType::ETwoHandHammer:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_TwoHandHammer")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_TwoHandHammer")];
				break;
			case EWeaponType::ETwoHandSword:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_TwoHandSword")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_TwoHandSword")];
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
		Val = Val * HorizontalLookRate * World->GetDeltaSeconds();
		APawn::AddControllerYawInput(Val);
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
