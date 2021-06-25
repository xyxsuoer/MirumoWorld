// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XYXPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXPatrolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = XYX)
		class AXYXPatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = XYX)
		int32 PointIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = XYX)
		bool bReverseDirection = false;

	UFUNCTION(BlueprintCallable, Category = XYX)
		void UpdatePatrolIndex();

	UFUNCTION(BlueprintCallable, Category = XYX)
		FVector GetSplinePointLocation(int32 InPointIndex);

	UFUNCTION()
		bool IsPatrolPathValid();


};
