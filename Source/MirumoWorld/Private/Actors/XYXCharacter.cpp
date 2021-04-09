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
#include "Kismet/KismetSystemLibrary.h"



// Sets default values
AXYXCharacter::AXYXCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 450;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset.Z = 180.f;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);
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
		PlayerInputComponent->BindAction("ToggleMovement", IE_Pressed, this, &AXYXCharacter::ToggleMovementAction);
	}
}

// Called when the game starts or when spawned
void AXYXCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitialzeCharacter();
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
	if (State == EMovementState::ESprint)
	{
		GetWorld()->GetTimerManager().SetTimer(SprintLoopTimer, this, &AXYXCharacter::SprintLoop, 0.016f, true);
	}
}

void AXYXCharacter::HandleMovementStateEnd(EMovementState State)
{
	if (State == EMovementState::ESprint)
	{
		GetWorld()->GetTimerManager().ClearTimer(SprintLoopTimer);
	}
}

void AXYXCharacter::MeleeAttack(EMeleeAttackType AttackType)
{
	if (!CanMeleeAttack() || !GetWorld())
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

	GetWorld()->GetTimerManager().ClearTimer(ResetMeleeAttackCounterTimer);
	
	UAnimMontage* Montage = GetMeleeAttackMontage(MeleeAttackType);
	if (Montage)
	{
		if (GetMesh() && GetMesh()->GetAnimInstance())
		{
			float Duration = -1.f;
			float AttackSpeed = 1.0f;
			Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage, AttackSpeed, EMontagePlayReturnType::Duration);
			if (Duration > 0.f)
			{
				Duration *= 0.99f;
				GetWorld()->GetTimerManager().SetTimer(ResetMeleeAttackCounterTimer, this, &AXYXCharacter::ResetMeleeAttackCounter, Duration, false);
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
	if (StateManagerComp && StateManagerComp->GetState() == EState::EIdle)
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
	EMontageAction StunDirection;

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
	
	if (!Montage && StunDirection != EMontageAction::EStunFront)
	{
		if (MontageManagerComp)
			Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EStunFront, 0);
	}

	return Montage;
}

UAnimMontage* AXYXCharacter::GetBlockMontage()
{
	UAnimMontage* Montage = nullptr;

	bool IsShieldEquipped = false;
	if (MontageManagerComp)
		Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EBlock, IsShieldEquipped ? 1 : 0);

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

	bool IsShieldEquipped = false;
	if (MontageManagerComp)
		Montage = MontageManagerComp->GetMontageForAction(EMontageAction::EParry, IsShieldEquipped ? 1 : 0);

	return Montage;
}

void AXYXCharacter::LightAttack()
{
	if (InputBufferComp)
	{
		InputBufferComp->UpdateKey(EInputBufferKey::ELightAttack);
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
	SetSprint(true);
}

void AXYXCharacter::StopSprintAction()
{
	SetSprint(false);
}

void AXYXCharacter::ToggleMovementAction()
{
	if (MovementSpeedComp)
	{
		MovementSpeedComp->ToggleState();
	}
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
	if (Montage)
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

void AXYXCharacter::SetSprint(bool bActivate)
{
	if (bActivate)
	{
		if (MovementSpeedComp)
		{
			StoredMovementState = MovementSpeedComp->GetMovementState();
			MovementSpeedComp->SetMovementState(EMovementState::ESprint);
		}
	}
	else
	{
		if (UKismetSystemLibrary::K2_IsTimerActive(nullptr, "SprintLoop"))
		{
			if (MovementSpeedComp)
			{
				MovementSpeedComp->SetMovementState(StoredMovementState);
			}
		}
	}
}

void AXYXCharacter::SprintLoop()
{
	if (MovementSpeedComp && MovementSpeedComp->GetMovementState() == EMovementState::ESprint)
	{
		if (GetVelocity().Size() > 10.f && IsIdleAndNotFalling())
		{
			// ÒÆ³ýÌåÁ¦Öµ
		}
	}
	else
	{
		SetSprint(false);
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

	UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		if (CommonMontages.Contains(Action)) 
		{
			if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Common")))
				DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Common")];
		}
		else
		{
			switch (CurrentWeaponType)
			{
			case EWeaponType::ENone:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Unarmed")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Unarmed")];
				break;
			case EWeaponType::EBow:
				if (GameInstance->MontageDataTables.Contains(TEXT("Heroe_Bow")))
					DataTable = GameInstance->MontageDataTables[TEXT("Heroe_Bow")];
				break;
			case EWeaponType::EDualSword:
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
			case EWeaponType::ETwinDagger:
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
	if (Val != 0.f && GetWorld())
	{
		Val = Val * HorizontalLookRate * GetWorld()->GetDeltaSeconds();
		APawn::AddControllerYawInput(Val);
	}
}

void AXYXCharacter::AddControllerPitchInput(float Val)
{
	if (Val != 0.f && GetWorld())
	{
		Val = Val * VerticalLookRate * GetWorld()->GetDeltaSeconds();
		APawn::AddControllerPitchInput(Val);
	}
}
