// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XYXRotatingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotatingStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotatingEnd);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXRotatingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXRotatingComponent();

	UPROPERTY(BlueprintAssignable)
		FOnRotatingStart OnRotatingStart;

	UPROPERTY(BlueprintAssignable)
		FOnRotatingEnd OnRotatingEnd;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float TimeElapsed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float RotateTime = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float MaxDegreesPerSecond = 720.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float MaxAllowedDegrees = 180.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bShouldRotate = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void SetRotationMode(ERotationMode Mode);

	UFUNCTION()
		bool IsRotating();

	UFUNCTION()
		void StartRotatingWithLimit(float MaxPossibleRotation, float InMaxDegreesPerSecond);

	UFUNCTION()
		void StopRotating();

	UFUNCTION()
		void StartRotatingWithTime(float InTime, float InMaxDegreesPerSecond);
};
