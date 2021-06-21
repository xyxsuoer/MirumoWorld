// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXMeleeAIAnimInstance.h"
#include "Actors/XYXBaseNPC.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Components/XYXStateManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>


void UXYXMeleeAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CharacterReference = Cast<AXYXBaseNPC>(TryGetPawnOwner());
	if (CharacterReference)
	{
		MovementComp = CharacterReference->GetCharacterMovement();
		EquipmentComp = CharacterReference->GetEquipmentManagerComponent();
		StateManagerComp = CharacterReference->GetStateManagerComponent();
		if (EquipmentComp)
			EquipmentComp->OnInCombatChanged.AddDynamic(this, &UXYXMeleeAIAnimInstance::HandleOnInCombatChanged);
		if(StateManagerComp)
			StateManagerComp->OnActivityChanged.AddDynamic(this, &UXYXMeleeAIAnimInstance::HandleOnActivityChanged);
	}
}

void UXYXMeleeAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CharacterReference && MovementComp && GetWorld())
	{
		bIsInAir = MovementComp->IsFalling();
		FVector Velocity = CharacterReference->GetVelocity();
		Speed = Velocity.Size();
		FRotator Rotation = CharacterReference->GetActorRotation();
		Direction = CalculateDirection(Velocity, Rotation);

		BlockAlpha = UKismetMathLibrary::FInterpTo_Constant(BlockAlpha, bIsBlocking ? 1 : 0, GetWorld()->GetDeltaSeconds(), 5.0f);
	}
}

void UXYXMeleeAIAnimInstance::HandleOnInCombatChanged(bool bInCombat)
{
	bIsInCombat = bInCombat;
}

void UXYXMeleeAIAnimInstance::HandleOnActivityChanged(EActivity Activity, bool Value)
{
	if (Activity == EActivity::EIsBlockingPressed)
	{
		bIsBlocking = Value;
	}
}