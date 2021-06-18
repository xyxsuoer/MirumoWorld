// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/XYXPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
AXYXPatrolPath::AXYXPatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SceneComp->SetupAttachment(RootComponent);

	PatrolSplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Patrol Spline Comp"));
	PatrolSplineComp->SetupAttachment(SceneComp);

	BillboardComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard Comp"));
	BillboardComp->SetupAttachment(SceneComp);

	PatrolSplineComp->SetClosedLoop(bCloseLoop, true);
}

// Called when the game starts or when spawned
void AXYXPatrolPath::BeginPlay()
{
	Super::BeginPlay();
}

