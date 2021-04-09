// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/XYXData.h"
#include "Interfaces/XYXInterfaceEntity.h"
#include "GenericTeamAgentInterface.h"
#include "XYXActor.generated.h"

UCLASS()
class MIRUMOWORLD_API AXYXActor : public AActor,
	public IGenericTeamAgentInterface, public IXYXInterfaceEntity
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXYXActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = XYX)
		ETeam CombatTeam = ETeam::EEnemiesTeam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;

	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		ETeam GetEntityCombatTeam();
	virtual ETeam GetEntityCombatTeam_Implementation() override {
		return CombatTeam;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool IsEntityAlive();
	virtual bool IsEntityAlive_Implementation() override {
		return !bIsDead;
	}


private:

	UPROPERTY(Replicated)
		bool bIsDead = false;


};
