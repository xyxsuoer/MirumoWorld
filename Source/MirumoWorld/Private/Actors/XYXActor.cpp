// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/XYXActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AXYXActor::AXYXActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AXYXActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AXYXActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AXYXActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AXYXActor, bIsDead);
}

void AXYXActor::SetGenericTeamId(const FGenericTeamId& InTeamID) {
	CombatTeam = (ETeam)InTeamID.GetId();
}

FGenericTeamId AXYXActor::GetGenericTeamId() const {
	return uint8(CombatTeam);
}
