// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXElvenBowAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include <Actors/XYXCharacter.h>

void UXYXElvenBowAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CurCharacter = Cast<ACharacter>(GetOwningActor()->GetOwner());
	DefaultStringLocation = StringLocation;
}

void UXYXElvenBowAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	IXYXInterfaceArcher* ArcherCharacter = Cast<IXYXInterfaceArcher>(CurCharacter);
	if (ArcherCharacter)
	{
		AimAlpha = ArcherCharacter->Execute_GetAimAlpha(CurCharacter);
		UpdateStringLocation();
	}
}

void UXYXElvenBowAnimInstance::UpdateStringLocation()
{
	IXYXInterfaceArcher* ArcherCharacter = Cast<IXYXInterfaceArcher>(CurCharacter);
	if (ArcherCharacter && GetWorld())
	{
		FVector TmpTargetLocation;
		if (ArcherCharacter->Execute_DoesHoldBowString(CurCharacter))
		{
			FVector TmpLocation = CurCharacter->GetMesh()->GetSocketLocation(ArcherCharacter->Execute_GetBowStringSocketName(CurCharacter));
			TmpTargetLocation =	UKismetMathLibrary::InverseTransformLocation(GetOwningComponent()->GetComponentTransform(), TmpLocation);
		}
		else
		{
			TmpTargetLocation = DefaultStringLocation;
		}

		StringLocation = UKismetMathLibrary::VInterpTo_Constant(StringLocation, TmpTargetLocation, GetWorld()->GetDeltaSeconds(), 1500.0f);

	}
}
