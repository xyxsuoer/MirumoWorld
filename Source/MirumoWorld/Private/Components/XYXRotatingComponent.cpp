// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXRotatingComponent.h"
#include "Actors/XYXCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/XYXInterfaceEntity.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UXYXRotatingComponent::UXYXRotatingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UXYXRotatingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	/*IXYXInterfaceEntity* Entity = Cast<IXYXInterfaceEntity>(GetOwner());
	if (!Entity)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Warning! %s does not implement interface I_RotatingComponent"), *GetOwner()->GetClass()->GetDisplayNameText().ToString()));
	}*/
}

// Called every frame
void UXYXRotatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bShouldRotate)
	{
		UWorld* const World = GetWorld();
		check(World);
		TimeElapsed += World->GetDeltaSeconds();
		if (TimeElapsed <= RotateTime)
		{
			IXYXInterfaceEntity* Entity = Cast<IXYXInterfaceEntity>(GetOwner());
			if (Entity)
			{
				FRotator TargetRotation =  Entity->Execute_GetDesiredRotation(GetOwner());
				FRotator TmpRotation = UKismetMathLibrary::RInterpTo_Constant(GetOwner()->GetActorRotation(), TargetRotation, World->GetDeltaSeconds(), MaxDegreesPerSecond);
				GetOwner()->SetActorRotation(TmpRotation);
			}
		}
	}
}

void UXYXRotatingComponent::SetRotationMode(ERotationMode Mode)
{
	auto CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		switch(Mode)
		{
		case ERotationMode::EFaceCamera: 
			{
				CharacterOwner->bUseControllerRotationYaw = false;
				CharacterOwner->GetCharacterMovement()->bUseControllerDesiredRotation = true;
				CharacterOwner->GetCharacterMovement()->bOrientRotationToMovement = false;
			}
			break;
		case ERotationMode::EOrientToMovement: 
			{			
				CharacterOwner->bUseControllerRotationYaw = false;
				CharacterOwner->GetCharacterMovement()->bUseControllerDesiredRotation = false;
				CharacterOwner->GetCharacterMovement()->bOrientRotationToMovement = true;		
			}
			break;
		}
	}
}

bool UXYXRotatingComponent::IsRotating()
{
	return bShouldRotate;
}


void UXYXRotatingComponent::StopRotating()
{
	bShouldRotate = false;
	OnRotatingEnd.Broadcast();
}

void UXYXRotatingComponent::StartRotatingWithTime(float InTime, float InMaxDegreesPerSecond)
{
	MaxDegreesPerSecond = InMaxDegreesPerSecond;
	RotateTime = InTime;
	TimeElapsed = 0.f;
	bShouldRotate = true;
	OnRotatingStart.Broadcast();
}

void UXYXRotatingComponent::StartRotatingWithLimit(float MaxPossibleRotation, float InMaxDegreesPerSecond)
{
	RotateTime = MaxPossibleRotation / InMaxDegreesPerSecond;
	MaxDegreesPerSecond = InMaxDegreesPerSecond;
	TimeElapsed = 0.f;
	bShouldRotate = true;
	OnRotatingStart.Broadcast();
}
