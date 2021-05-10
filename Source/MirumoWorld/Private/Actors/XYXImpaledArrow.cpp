// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/XYXImpaledArrow.h"

// Sets default values
AXYXImpaledArrow::AXYXImpaledArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	Scene->SetupAttachment(RootComponent);

	ImpaledArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	ImpaledArrowMesh->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void AXYXImpaledArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AXYXImpaledArrow::SetArrowMesh(class UStaticMesh* Mesh)
{
	if (ImpaledArrowMesh)
	{
		ImpaledArrowMesh->SetStaticMesh(Mesh);
	}

	SetLifeSpan(LifeTime);

	if (AActor* NewOwner = GetOwner())
	{
		NewOwner->OnDestroyed.AddDynamic(this, &AXYXImpaledArrow::OnOwnerDestroyed);
	}
}

void AXYXImpaledArrow::OnOwnerDestroyed(AActor* DestroyedActor)
{
	UWorld* World = GetWorld();
	if (World) 
		World->DestroyActor(this);
}
