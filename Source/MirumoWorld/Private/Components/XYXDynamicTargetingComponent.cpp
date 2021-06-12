// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXDynamicTargetingComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/XYXInterfaceTargetable.h"


// Sets default values for this component's properties
UXYXDynamicTargetingComponent::UXYXDynamicTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UXYXDynamicTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	SetDebugMode();
	CheckCollisionTypeArrays();
}


// Called every frame
void UXYXDynamicTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCameraLock();
}

void UXYXDynamicTargetingComponent::FindTarget()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithInterface(this, UXYXInterfaceTargetable::StaticClass(), OutActors);
	AActor* LocalPotentialTarget = nullptr;
	TArray<AActor*> PotentialTargets = {};
	float DistanceFromCenterOfViewport = 0.f;
	for (auto&& e : OutActors)
	{
		LocalPotentialTarget = e;
		IXYXInterfaceTargetable* SelectTarget = Cast<IXYXInterfaceTargetable>(LocalPotentialTarget);
		if (SelectTarget && SelectTarget->Execute_IsTargetable(LocalPotentialTarget) && GetOwner() != SelectedActor)
		{
			FVector2D ScreenPosition;
			bool bRetunValue;

			GetActorScreenPosition(LocalPotentialTarget, ScreenPosition, bRetunValue);
			if (bRetunValue && GetDistanceToOwner(LocalPotentialTarget) <= TargetingMaxDistance && IsInViewport(ScreenPosition))
			{
				if (!IsAnythingBlockingTrace(LocalPotentialTarget, PotentialTargets))
				{
					PotentialTargets.Add(LocalPotentialTarget);
				}
			}
		}
	}

	for (int32 i = 0; i < PotentialTargets.Num(); ++i)
	{
		LocalPotentialTarget = PotentialTargets[i];
		if (i == 0)
		{
			FVector2D ScreenPosition;
			bool bRetunValue;
			GetActorScreenPosition(LocalPotentialTarget, ScreenPosition, bRetunValue);
			FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);
			DistanceFromCenterOfViewport = UKismetMathLibrary::Abs(ScreenPosition.X - ViewportSize.X);
			SelectedActor = LocalPotentialTarget;
		}
		else
		{
			FVector2D ScreenPosition;
			bool bRetunValue;
			GetActorScreenPosition(LocalPotentialTarget, ScreenPosition, bRetunValue);
			FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);
			if (UKismetMathLibrary::Abs(ScreenPosition.X - ViewportSize.X) < DistanceFromCenterOfViewport)
			{
				DistanceFromCenterOfViewport = UKismetMathLibrary::Abs(ScreenPosition.X - ViewportSize.X);
				SelectedActor = LocalPotentialTarget;
			}
		}
	}

	if (IsValid(SelectedActor))
	{
		EnableCameraLock();
		OnTargetChanged.Broadcast(SelectedActor);
	}
}

bool UXYXDynamicTargetingComponent::IsInViewport(FVector2D ScreenPosition)
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	return ScreenPosition.X > 0.f && ScreenPosition.Y > 0.f && ScreenPosition.X <= ViewportSize.X && ScreenPosition.Y <= ViewportSize.Y;
}

FVector UXYXDynamicTargetingComponent::GetLineTraceStartLocation()
{
	FVector StartLocation = FVector::ZeroVector;
	if (GetOwner())
	{
		StartLocation = GetOwner()->GetActorLocation() +
			GetOwner()->GetActorForwardVector() * TraceDepthOffset +
			GetOwner()->GetActorUpVector() * TraceHeightOffset;
	}

	return StartLocation;
}

void UXYXDynamicTargetingComponent::EnableCameraLock()
{
	bIsTargetingEnabled = true;
	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(DisableCameraLockHandle);
	SetDebugMode();
	IXYXInterfaceTargetable* SelectTarget = Cast<IXYXInterfaceTargetable>(SelectedActor);
	if (SelectTarget)
	{
		SelectTarget->Execute_OnSelected(SelectedActor);
		World->GetTimerManager().SetTimer(CheckTargetHandle, this, &UXYXDynamicTargetingComponent::CheckTarget, 0.15f, true);
		UpdateIgnoreLookInput();
		OnTargetingToggled.Broadcast(true);
	}
}

void UXYXDynamicTargetingComponent::UpdateCameraLock()
{
	if (!IsTargetingEnabled() || !GetOwner() || !GetWorld())
	{
		return;
	}

	if (GetDistanceToOwner(SelectedActor) >= 50.f && GetDistanceToOwner(SelectedActor) <= TargetingMaxDistance)
	{
		if (IsValid(ArrowComp))
		{
			FRotator Tmp;
			Tmp.Roll = 0.f;
			Tmp.Pitch = 0.f;
			Tmp.Yaw = UKismetMathLibrary::RInterpTo_Constant(ArrowComp->GetComponentRotation(), 
				GetOwner()->GetInstigator()->GetControlRotation(), GetWorld()->GetDeltaSeconds(), 250.f).Yaw;
			ArrowComp->SetWorldRotation(Tmp);
		}

		if (!bIsFreeCamera)
		{
			FRotator Tmp =UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), SelectedActor->GetActorLocation());
			GetOwner()->GetInstigatorController()->SetControlRotation(
				UKismetMathLibrary::RInterpTo_Constant(
					GetOwner()->GetInstigator()->GetControlRotation(), Tmp, GetWorld()->GetDeltaSeconds(),300.f
				)
			);
		}
	}
	else
	{
		DisableCameraLock();
	}
}

void UXYXDynamicTargetingComponent::SetDebugMode()
{
	DebugMode = bDebug ? EDrawDebugTrace::Type::ForDuration : EDrawDebugTrace::Type::None;
}

void UXYXDynamicTargetingComponent::CheckTarget()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Emplace(SelectedActor);

	UWorld* World = GetWorld();
	check(World);

	if (IsAnythingBlockingTrace(SelectedActor, ActorsToIgnore))
	{
		bool bTmpIsActive = World->GetTimerManager().IsTimerActive(DisableCameraLockHandle);
		if (!bTmpIsActive)
		{
			if (DisableOnBlockDelay == 0.f)
			{
				DisableCameraLock();
			}
			else
			{
				World->GetTimerManager().SetTimer(DisableCameraLockHandle, this, &UXYXDynamicTargetingComponent::DisableCameraLock, DisableOnBlockDelay, false);
			}
		}
	}
	else
	{
		World->GetTimerManager().ClearTimer(DisableCameraLockHandle);
		IXYXInterfaceTargetable* SelectTarget = Cast<IXYXInterfaceTargetable>(SelectedActor);
		if (SelectTarget && !SelectTarget->Execute_IsTargetable(SelectedActor))
		{
			DisableCameraLock();
			FindTarget();
		}
	}
}

void UXYXDynamicTargetingComponent::FindDirectionalTarget(bool bOnLeft)
{
	if (IsTargetingEnabled())
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsWithInterface(this, UXYXInterfaceTargetable::StaticClass(), OutActors);
		AActor* LocalPotentialTarget = nullptr;
		TArray<AActor*> PotentialTargetsRight;
		TArray<AActor*> PotentialTargetLeft;
		for (auto&& e : OutActors)
		{
			LocalPotentialTarget = e;
			IXYXInterfaceTargetable* SelectTarget = Cast<IXYXInterfaceTargetable>(LocalPotentialTarget);
			if (SelectTarget && SelectTarget->Execute_IsTargetable(LocalPotentialTarget) &&
				LocalPotentialTarget != SelectedActor && GetOwner() != SelectedActor)
			{
				if (GetDistanceToOwner(LocalPotentialTarget) <= TargetingMaxDistance)
				{
					if (!IsAnythingBlockingTrace(LocalPotentialTarget, PotentialTargetsRight))
					{
						if (!IsAnythingBlockingTrace(LocalPotentialTarget, PotentialTargetLeft))
						{
							if (GetTargetRightSide(LocalPotentialTarget))
							{
								PotentialTargetsRight.Add(LocalPotentialTarget);
							}
							else
							{
								PotentialTargetLeft.Add(LocalPotentialTarget);
							}
						}
					}
				}
			}
		}

		if (bOnLeft)
		{
			if (PotentialTargetLeft.Num() > 0)
			{
				LocalPotentialTarget = GetTargetByDotProduct(PotentialTargetLeft, true);
			}
			else
			{
				LocalPotentialTarget = GetTargetByDotProduct(PotentialTargetsRight, false);
			}
		}
		else
		{
			if (PotentialTargetsRight.Num() > 0)
			{
				LocalPotentialTarget = GetTargetByDotProduct(PotentialTargetsRight, true);
			}
			else
			{
				LocalPotentialTarget = GetTargetByDotProduct(PotentialTargetLeft, false);
			}
		}

		if (IsValid(LocalPotentialTarget))
		{
			IXYXInterfaceTargetable* SelectTarget = Cast<IXYXInterfaceTargetable>(SelectedActor);
			if (SelectTarget)
			{
				SelectTarget->Execute_OnDeselected(SelectedActor);
				SelectedActor = LocalPotentialTarget;
				SelectTarget = Cast<IXYXInterfaceTargetable>(SelectedActor);
				if (SelectTarget)
				{
					SelectTarget->Execute_OnSelected(SelectedActor);
					OnTargetChanged.Broadcast(SelectedActor);
				}
			}
		}
	}
}

AActor* UXYXDynamicTargetingComponent::GetTargetByDotProduct(TArray<AActor*>& InArray, bool bBest)
{
	float LocalDotProduct = 0.f;
	AActor* LocalPotentialTarget = nullptr;
	for (int32 i = 0; i < InArray.Num(); ++i)
	{
		if (i == 0)
		{
			LocalDotProduct = CalculateDotProductToTarget(InArray[i]);
			LocalPotentialTarget = InArray[i];
		}
		else
		{
			bool TmpCondition = bBest ? CalculateDotProductToTarget(InArray[i]) > LocalDotProduct : CalculateDotProductToTarget(InArray[i]) < LocalDotProduct;
			if (TmpCondition)
			{
				LocalDotProduct = CalculateDotProductToTarget(InArray[i]);
				LocalPotentialTarget = InArray[i];
			}
		}
	}

	return LocalPotentialTarget;
}

void UXYXDynamicTargetingComponent::UpdateIgnoreLookInput()
{
	if (GetOwner()->GetInstigatorController())
	{
		if (IsTargetingEnabled() && !bIsFreeCamera)
		{
			GetOwner()->GetInstigatorController()->SetIgnoreLookInput(true);
		}
		else
		{
			GetOwner()->GetInstigatorController()->ResetIgnoreLookInput();
		}
	}
}

bool UXYXDynamicTargetingComponent::IsAnythingBlockingTrace(AActor* Target, TArray<AActor*>& ActorsToIgnore)
{
	if (ActorsToIgnore.Num() > 0 && Target)
	{
		FHitResult HitResult;
		return	UKismetSystemLibrary::LineTraceSingleForObjects(this, GetLineTraceStartLocation(), Target->GetActorLocation(),
			BlockingCollisionTypes, false, ActorsToIgnore, DebugMode, HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.5f);
	}

	return false;
}

void UXYXDynamicTargetingComponent::CheckCollisionTypeArrays()
{
	for (auto&& e: AllowedCollisionTypes)
	{
		BlockingCollisionTypes.Remove(e);
	}
}

void UXYXDynamicTargetingComponent::GetActorScreenPosition(AActor* InActor, FVector2D& ScreenPosition, bool& ReturnValue)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (InActor && PlayerController)
	{
		ReturnValue = UGameplayStatics::ProjectWorldToScreen(PlayerController, InActor->GetActorLocation(), ScreenPosition, false);
	}
}

float UXYXDynamicTargetingComponent::GetDistanceToOwner(AActor* OtherActor)
{
	if (OtherActor)
	{
		return GetOwner()->GetDistanceTo(OtherActor);
	}

	return 0.f;
}

bool UXYXDynamicTargetingComponent::GetTargetRightSide(AActor* PotentialTarget)
{
	if (GetOwner() && SelectedActor && PotentialTarget)
	{
		FVector TmpVec1 = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), SelectedActor->GetActorLocation());
		FVector TmpVec2 = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), PotentialTarget->GetActorLocation());
		if (UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Cross_VectorVector(TmpVec1, TmpVec2), GetOwner()->GetActorUpVector()) >= 0.f)
		{
			return true; 
		}
	}

	return false; 
}

float UXYXDynamicTargetingComponent::CalculateDotProductToTarget(AActor* Target)
{
	if (GetOwner() && SelectedActor && Target)
	{
		FVector TmpVec1 = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), SelectedActor->GetActorLocation());
		FVector TmpVec2 = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), Target->GetActorLocation());
		return UKismetMathLibrary::Dot_VectorVector(TmpVec1, TmpVec2);
	}

	return 0.f;
}

void UXYXDynamicTargetingComponent::Initialize(UArrowComponent* InArrowComp)
{
	if (IsValid(InArrowComp))
	{
		ArrowComp = InArrowComp;
		ArrowComp->SetUsingAbsoluteLocation(true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Arrow component is not valid"));
	}
}

void UXYXDynamicTargetingComponent::DisableCameraLock()
{
 if (bIsTargetingEnabled)
 {
	 bIsTargetingEnabled = false;

	 IXYXInterfaceTargetable* SelectTarget = Cast<IXYXInterfaceTargetable>(SelectedActor);
	 if (SelectTarget)
	 {
		 SelectTarget->Execute_OnDeselected(SelectedActor);
		 SelectedActor = nullptr;
		 UWorld* World = GetWorld();
		 check(World)
		 World->GetTimerManager().ClearTimer(CheckTargetHandle);
		 UpdateIgnoreLookInput();
		 OnTargetingToggled.Broadcast(false);
	 }
 }
}

void UXYXDynamicTargetingComponent::FindTargetWithAxisInput(float AxisValue)
{
	if (!IsValid(ArrowComp) || !IsValid(SelectedActor) ||
		!(UKismetMathLibrary::Abs(AxisValue) > 1.5f) || bIsFreeCamera || 
		!GetWorld() || !GetOwner()->GetInstigator() || !ArrowComp)
	{
		return;
	}

	FRotator Tmp = UKismetMathLibrary::NormalizedDeltaRotator(GetOwner()->GetInstigator()->GetControlRotation(), ArrowComp->GetComponentRotation());
	if (UKismetMathLibrary::Abs(Tmp.Yaw) <= 165.f)
	{
		FRotator Tar;
		Tar.Roll = 0.f;
		Tar.Pitch = 0.f;
		Tar.Yaw = UKismetMathLibrary::Clamp(AxisValue * AxisInputSensitivity, -625.f * GetWorld()->GetDeltaSeconds(), 625.f * GetWorld()->GetDeltaSeconds());
		ArrowComp->AddLocalRotation(Tar);
	}

	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::CapsuleTraceMultiForObjects(this, ArrowComp->GetComponentLocation(),
		ArrowComp->GetComponentLocation() + ArrowComp->GetForwardVector() * TargetingMaxDistance, 32.f, 
		TargetingMaxHeight, AllowedCollisionTypes, false, ActorsToIgnore, DebugMode, HitResults, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

	for (auto&& e : HitResults)
	{
		IXYXInterfaceTargetable* HitActor = Cast<IXYXInterfaceTargetable>(e.GetActor());
		IXYXInterfaceTargetable* SelectTarget = Cast<IXYXInterfaceTargetable>(SelectedActor);
		if (HitActor && e.bBlockingHit)
		{
			if (SelectedActor == e.GetActor())
			{
				break;
			}

			if (!IsAnythingBlockingTrace(e.GetActor(), ActorsToIgnore))
			{
				if (HitActor->Execute_IsTargetable(e.GetActor()) && SelectTarget)
				{
					SelectTarget->Execute_OnDeselected(SelectedActor);
					SelectedActor = e.GetActor();
					SelectTarget = Cast<IXYXInterfaceTargetable>(SelectedActor);
					if (SelectTarget)
					{
						SelectTarget->Execute_OnSelected(SelectedActor);
						OnTargetChanged.Broadcast(SelectedActor);
						break;
					}
				}
			}
		}
	}
}

void UXYXDynamicTargetingComponent::ToggleCameraLock()
{
	if (bIsTargetingEnabled)
	{
		DisableCameraLock();
	}
	else
	{
		FindTarget();
	}
}

bool UXYXDynamicTargetingComponent::IsTargetingEnabled()
{
	return bIsTargetingEnabled;
}

void UXYXDynamicTargetingComponent::FindTargetOnLeft()
{
	FindDirectionalTarget(true);
}

void UXYXDynamicTargetingComponent::FindTargetOnRight()
{
	FindDirectionalTarget(false);
}

void UXYXDynamicTargetingComponent::SetFreeCamera(bool bFreeCamera)
{
	bIsFreeCamera = bFreeCamera;
	UpdateIgnoreLookInput();
}

