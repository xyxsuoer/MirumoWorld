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

void UXYXCollisionHandlerComponent::SetCollisionMesh(UPrimitiveComponent* WeaponMesh, TArray<FName>& InSockets)
{
	Mesh = WeaponMesh;
	Sockets = InSockets;
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
	if (IsValid(Mesh))
	{
		for (auto&& e : Sockets)
		{
			LastSocketLocations.Add(e, Mesh->GetSocketLocation(e));
		}
	}
}

void UXYXCollisionHandlerComponent::PerformTrace()
{
	if (IsValid(Mesh))
	{
		for (auto&& e : Sockets)
		{
			if (LastSocketLocations.Contains(e))
			{
				FVector TmpStartLocation = LastSocketLocations[e];
				FVector TmpEndLocation = Mesh->GetSocketLocation(e);
				UWorld* const World = GetWorld();
				check(World);
				TArray<FHitResult> OutHits;
				UKismetSystemLibrary::SphereTraceMultiForObjects(
					World, TmpStartLocation, TmpEndLocation, TraceRadius, 
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

