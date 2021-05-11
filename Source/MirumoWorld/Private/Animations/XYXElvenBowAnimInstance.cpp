// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXElvenBowAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include <Actors/XYXCharacter.h>

void UXYXElvenBowAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	XYXCharacter = Cast<AXYXCharacter>(GetOwningActor()->GetOwner());
	DefaultStringLocation = StringLocation;
}

void UXYXElvenBowAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (XYXCharacter)
	{
		AimAlpha = XYXCharacter->GetAimAlpha();
		UpdateStringLocation();
	}
}

void UXYXElvenBowAnimInstance::UpdateStringLocation()
{
	if (XYXCharacter && GetWorld())
	{
		FVector TmpTargetLocation;
		if (XYXCharacter->DoesHoldBowString())
		{
			FVector TmpLocation = XYXCharacter->GetMesh()->GetSocketLocation(XYXCharacter->GetBowStringSocketName());
			TmpTargetLocation =	UKismetMathLibrary::InverseTransformLocation(GetOwningComponent()->GetComponentTransform(), TmpLocation);
		}
		else
		{
			TmpTargetLocation = DefaultStringLocation;
		}

		StringLocation = UKismetMathLibrary::VInterpTo_Constant(StringLocation, TmpTargetLocation, GetWorld()->GetDeltaSeconds(), 1500.0f);

	}
}
