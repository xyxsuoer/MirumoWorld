// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/XYXData.h"
#include "XYXMovementSpeedComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementStateStart, EMovementState, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementStateEnd, EMovementState, State);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXMovementSpeedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXMovementSpeedComponent();

	UPROPERTY(BlueprintAssignable)
		FOnMovementStateStart OnMovementStateStart;

	UPROPERTY(BlueprintAssignable)
		FOnMovementStateEnd OnMovementStateEnd;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class ACharacter* CharacterOwner;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsUpdatingSpeed = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EMovementState MovementState = EMovementState::EIdle;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EMovementState StartMovementState = EMovementState::EJog;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float CrouchSpeed = 100.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float WalkSpeed = 200.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float JogSpeed = 500.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float SprintSpeed = 700.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		float TargetSpeed = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = XYX)
		float SpeedChangeInterpSpeed = 3.f;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<EMovementState> StateToToggle = { 
			EMovementState::EWalk, 
			EMovementState::EJog,
		};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		EMovementState GetMovementState() { return MovementState; }

	UFUNCTION()
		void SetMovementState(EMovementState State);

	UFUNCTION()
		void UpdateMaxSpeed();

	UFUNCTION()
		void ToggleState();
		
};
