// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXAnimInstance.h"
#include "Actors/XYXCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/NavMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/XYXMovementSpeedComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "Kismet/GameplayStatics.h"

void UXYXAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SetReferences();
	

}

void UXYXAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (XYXCharacter)
	{
		StoreCharacterInfo();
	}

}

void UXYXAnimInstance::SetReferences()
{
	XYXCharacter = Cast<AXYXCharacter>(TryGetPawnOwner());
	if (XYXCharacter)
	{
		MovementComp = XYXCharacter->GetCharacterMovement();
	}
}


void UXYXAnimInstance::StoreCharacterInfo()
{
	if (XYXCharacter && MovementComp)
	{
		bIsInAir = MovementComp->IsFalling();

		Velocity = XYXCharacter->GetVelocity();
		Speed = Velocity.Size();
		Rotation = XYXCharacter->GetActorRotation();
		Direction = CalculateDirection(Velocity, Rotation);

		bIsInSlowMotion = XYXCharacter->GetIsInSlowMotion();
		BlockAlpha = XYXCharacter->GetBlockAlpha();
		AimAlpha = XYXCharacter->GetAimAlpha();
		KatanaStance = XYXCharacter->GetKatanaStance();
		if (XYXCharacter->GetMovementSpeedComponent()) 
		{
			MovementState = XYXCharacter->GetMovementSpeedComponent()->GetMovementState();
			bIsSprinting =	MovementState == EMovementState::ESprint;
			bIsCrouching = MovementState == EMovementState::ECrouch;
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			float MouseX = 0.0f;
			float MouseY = 0.0f;
			PlayerController->GetInputMouseDelta(MouseX, MouseY);
			MouseDeltaX = MouseX;
		}
	}
}


