// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXArcherAIAnimInstance.h"
#include "Actors/XYXBaseNPC.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "Interfaces/XYXInterfaceArcher.h"

void UXYXArcherAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CharacterReference = Cast<AXYXBaseNPC>(TryGetPawnOwner());
	if (CharacterReference)
	{
		MovementComp = CharacterReference->GetCharacterMovement();
		EquipmentComp = CharacterReference->GetEquipmentManagerComponent();
		if (EquipmentComp)
			EquipmentComp->OnInCombatChanged.AddDynamic(this, &UXYXArcherAIAnimInstance::HandleOnInCombatChanged);
	}
}

void UXYXArcherAIAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterReference && MovementComp && GetWorld())
	{
		bIsInAir = MovementComp->IsFalling();
		FVector Velocity = CharacterReference->GetVelocity();
		Speed = Velocity.Size();
		FRotator Rotation = CharacterReference->GetActorRotation();
		Direction = CalculateDirection(Velocity, Rotation);

		IXYXInterfaceArcher* ArcherCharacter = Cast<IXYXInterfaceArcher>(CharacterReference);
		if (ArcherCharacter)
		{
			AimAlpha = ArcherCharacter->Execute_GetAimAlpha(CharacterReference);
		}
	}
}

void UXYXArcherAIAnimInstance::HandleOnInCombatChanged(bool bInCombat)
{
	bIsInCombat = bInCombat;
}
