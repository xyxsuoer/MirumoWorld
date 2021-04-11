// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXMovementSpeedComponent.h"
#include "Actors/XYXCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UXYXMovementSpeedComponent::UXYXMovementSpeedComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UXYXMovementSpeedComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<AXYXCharacter>(GetOwner());
	MovementState = StartMovementState;
	SetMovementState(MovementState);
}


// Called every frame
void UXYXMovementSpeedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMaxSpeed();
}

void UXYXMovementSpeedComponent::SetMovementState(EMovementState State)
{
	if (CharacterOwner && CharacterOwner->GetCharacterMovement())
	{
		OnMovementStateEnd.Broadcast(MovementState);
		MovementState = State;
		switch(MovementState)
		{
		case EMovementState::EIdle:
			TargetSpeed = 0.f;
			break;
		case EMovementState::ECrouch:
			TargetSpeed = CrouchSpeed;
			break;
		case EMovementState::EWalk:
			TargetSpeed = WalkSpeed;
			break;
		case EMovementState::EJog:
			TargetSpeed = JogSpeed;
			break;
		case EMovementState::ESprint:
			TargetSpeed = SprintSpeed;
			break;
		}
		bIsUpdatingSpeed = true;
		OnMovementStateStart.Broadcast(MovementState);
	}
}

void UXYXMovementSpeedComponent::UpdateMaxSpeed()
{
	if (!bIsUpdatingSpeed || !GetWorld())
	{
		return;
	}

	if (CharacterOwner)
	{
		UCharacterMovementComponent* MovementComp = CharacterOwner->GetCharacterMovement();
		if (MovementComp)
		{
			float CurMaxWalkSpeed = MovementComp->MaxWalkSpeed;
			MovementComp->MaxWalkSpeed = FMath::FInterpTo(CurMaxWalkSpeed, TargetSpeed, GetWorld()->GetDeltaSeconds(), SpeedChangeInterpSpeed);

			if (FMath::IsNearlyEqual(MovementComp->MaxWalkSpeed, TargetSpeed, 1.f))
			{
				MovementComp->MaxWalkSpeed = TargetSpeed;
				bIsUpdatingSpeed = false;
			}
		}
	}
}

void UXYXMovementSpeedComponent::ToggleState()
{
	int32 Index = 0;
	if (StateToToggle.Contains(MovementState))
	{
		Index = StateToToggle.Find(MovementState);
		if (Index >= 0)
		{
			int32 tempIndex = 0;
			if (Index < StateToToggle.Num() - 1)
			{
				tempIndex = Index + 1;
			}
			SetMovementState(StateToToggle[tempIndex]);
		}
	}
}

