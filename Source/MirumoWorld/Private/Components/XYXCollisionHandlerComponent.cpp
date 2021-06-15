// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXCollisionHandlerComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UXYXCollisionHandlerComponent::UXYXCollisionHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UXYXCollisionHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCollisionActive)
	{
		if (bCanPerformTrace)
		{
			PerformTrace();
		}

		UpdateLastSocketPosition();
		bCanPerformTrace = true;
	}
}

bool UXYXCollisionHandlerComponent::IsIgnoredClass(TSubclassOf<AActor> TestClass)
{
	for (auto&& e : IgnoreClasses)
	{
		if (UKismetMathLibrary::ClassIsChildOf(TestClass, e))
		{
			return true;
		}
	}
	return false;
}

bool UXYXCollisionHandlerComponent::IsCollisionActive()
{
	return bCanPerformTrace;
}

void UXYXCollisionHandlerComponent::SetCollisionMesh(class UPrimitiveComponent* WeaponMesh, TArray<FName> InSockets)
{
	MeshSockets.Empty();

	CollisionMesh = WeaponMesh;
	MeshSockets = InSockets;
	UpdateLastSocketPosition();
}

void UXYXCollisionHandlerComponent::SetDualCollisionMesh(class UPrimitiveComponent* WeaponMesh, TArray<FName> InSockets)
{
	DualMeshSockets.Empty();

	DualCollisionMesh = WeaponMesh;
	DualMeshSockets = InSockets;
	UpdateLastSocketPosition();
}

void UXYXCollisionHandlerComponent::ActiveCollision(ECollisionPart CollisionPart)
{
	ActorsHit.Empty();
	ActorsHit.Emplace(GetOwner());
	bCollisionActive = true;
	OnCollisionActivated.Broadcast(CollisionPart);
}

void UXYXCollisionHandlerComponent::DeactiveCollision()
{
	bCollisionActive = false;
	bCanPerformTrace = false;
	OnCollisionDeactivated.Broadcast();
}

void UXYXCollisionHandlerComponent::UpdateLastSocketPosition()
{
	if (IsValid(CollisionMesh))
	{
		for (auto&& e : MeshSockets)
		{
			LastSocketLocations.Add(e, CollisionMesh->GetSocketLocation(e));
		}
	}

	if (IsValid(DualCollisionMesh))
	{
		for (auto&& e : DualMeshSockets)
		{
			LastSocketLocations.Add(e, DualCollisionMesh->GetSocketLocation(e));
		}
	}
}

void UXYXCollisionHandlerComponent::PerformTrace()
{
	UWorld* const World = GetWorld();
	check(World);

	if (IsValid(CollisionMesh))
	{
		for (auto&& e : MeshSockets)
		{
			if (LastSocketLocations.Contains(e))
			{
				FVector TmpStartLocation = LastSocketLocations[e];
				FVector TmpEndLocation = CollisionMesh->GetSocketLocation(e);
				TArray<FHitResult> OutHits;
				UKismetSystemLibrary::SphereTraceMultiForObjects(
					this, TmpStartLocation, TmpEndLocation, TraceRadius, 
					ObjectTypesToCollideWith, false, ActorsHit, VisualizeTrace, OutHits, true
				);

				for (auto&& o : OutHits)
				{
					if (!ActorsHit.Contains(o.GetActor()) && 
						!IsIgnoredClass(o.Actor->GetClass()) && 
						!IgnoredCollisionProfileNames.Contains(o.GetComponent()->GetCollisionProfileName()))
					{
						ActorsHit.Add(o.GetActor());
						OnHit.Broadcast(o);
					}
				}
			}
		}
	}

	if (IsValid(DualCollisionMesh))
	{
		for (auto&& e : DualMeshSockets)
		{
			if (LastSocketLocations.Contains(e))
			{
				FVector TmpStartLocation = LastSocketLocations[e];
				FVector TmpEndLocation = DualCollisionMesh->GetSocketLocation(e);
				TArray<FHitResult> OutHits;
				UKismetSystemLibrary::SphereTraceMultiForObjects(
					this, TmpStartLocation, TmpEndLocation, TraceRadius,
					ObjectTypesToCollideWith, false, ActorsHit, VisualizeTrace, OutHits, true
				);

				for (auto&& o : OutHits)
				{
					if (!ActorsHit.Contains(o.GetActor()) &&
						!IsIgnoredClass(o.Actor->GetClass()) &&
						!IgnoredCollisionProfileNames.Contains(o.GetComponent()->GetCollisionProfileName()))
					{
						ActorsHit.Add(o.GetActor());
						OnHit.Broadcast(o);
					}
				}
			}
		}
	}
}

