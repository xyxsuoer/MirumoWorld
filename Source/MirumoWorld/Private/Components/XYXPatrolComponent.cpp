// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXPatrolComponent.h"
#include "Actors/XYXPatrolPath.h"
#include "Components/SplineComponent.h"

// Sets default values for this component's properties
UXYXPatrolComponent::UXYXPatrolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UXYXPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	PatrolPath = NewObject<AXYXPatrolPath>(this, PatrolPathClass);
}


void UXYXPatrolComponent::UpdatePatrolIndex()
{
	if (!PatrolPath || !PatrolPath->PatrolSplineComp)
	{
		return;
	}

	if (bReverseDirection)
	{
		if (PointIndex <= 0)
		{
			if (PatrolPath->PatrolSplineComp->IsClosedLoop())
			{
				PointIndex = PatrolPath->PatrolSplineComp->GetNumberOfSplinePoints() - 1;
			}
			else
			{
				PointIndex = 1;
				bReverseDirection = false;
			}

		}
		else
		{
			PointIndex--;
		}
	}
	else
	{
		if (PointIndex >= PatrolPath->PatrolSplineComp->GetNumberOfSplinePoints() - 1)
		{
			if (PatrolPath->PatrolSplineComp->IsClosedLoop())
			{
				PointIndex = 0;
			}
			else
			{
				bReverseDirection = true;
				PointIndex = PatrolPath->PatrolSplineComp->GetNumberOfSplinePoints() - 2;
			}
		}
		else
		{
			PointIndex++;
		}
	}
}

FVector UXYXPatrolComponent::GetSplinePointLocation(int32 InPointIndex)
{
	if (PatrolPath && PatrolPath->PatrolSplineComp)
	{
		return	PatrolPath->PatrolSplineComp->GetLocationAtSplinePoint(InPointIndex, ESplineCoordinateSpace::World);
	}

	return FVector::ZeroVector;
}

bool UXYXPatrolComponent::IsPatrolPathValid()
{
	return PatrolPath ? true : false;
}

