// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XYXPatrolPath.generated.h"

UCLASS()
class MIRUMOWORLD_API AXYXPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXYXPatrolPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = XYX)
		class USplineComponent* PatrolSplineComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = XYX)
		class UBillboardComponent* BillboardComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = XYX)
		class USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = XYX)
		bool bCloseLoop = false;
	
};
