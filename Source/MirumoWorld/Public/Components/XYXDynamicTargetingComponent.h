// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "XYXDynamicTargetingComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetChanged, AActor*, NewTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetingToggled, bool, bEnabled);

class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXDynamicTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXDynamicTargetingComponent();

	UPROPERTY(BlueprintAssignable)
		FOnTargetChanged OnTargetChanged;

	UPROPERTY(BlueprintAssignable)
		FOnTargetingToggled OnTargetingToggled;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class UArrowComponent* ArrowComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float AxisInputSensitivity = 1.5f;  // higher this value is, easier it will be to select new target on left or right

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float TargetingMaxDistance = 2500.f;  // Maximum distance at which targeting system will work, after exceeding this value, targeting system will be disabled

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float TargetingMaxHeight = 640.f;   // Maximum value of  selecting targets below or above character

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float TraceDepthOffset = 0.f;  // Offset added to start location of line trace checking potential targets, look at GetLineTraceStartLocation function

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float TraceHeightOffset = 100.f;  // Offset added to start location of line trace checking potential targets, look at GetLineTraceStartLocation function

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TArray<TEnumAsByte<EObjectTypeQuery>> AllowedCollisionTypes = {};  // Types allowed to be targeted, vlaues in this array shouldn't be in BlockingCollisionTypes

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TArray<TEnumAsByte<EObjectTypeQuery>> BlockingCollisionTypes = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float DisableOnBlockDelay = 2.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TEnumAsByte<EDrawDebugTrace::Type> DebugMode;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		AActor* SelectedActor;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsTargetingEnabled = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		bool bDebug = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FTimerHandle CheckTargetHandle; // handle of timer checking if selected actor is still targetable

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bIsFreeCamera = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		FTimerHandle DisableCameraLockHandle; 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void FindTarget();

	UFUNCTION()
		bool IsInViewport(FVector2D ScreenPosition);

	UFUNCTION()
		FVector GetLineTraceStartLocation();

	UFUNCTION()
		void EnableCameraLock();

	UFUNCTION()
		void UpdateCameraLock();

	UFUNCTION()
		void SetDebugMode();

	UFUNCTION()
		void CheckTarget();

	UFUNCTION()
		void FindDirectionalTarget(bool bOnLeft);

	UFUNCTION()
		AActor* GetTargetByDotProduct(TArray<AActor*>& InArray, bool bBest);

	UFUNCTION()
		void UpdateIgnoreLookInput();

	UFUNCTION()
		bool IsAnythingBlockingTrace(AActor* Target, TArray<AActor*>& ActorsToIgnore);

	UFUNCTION()
		void CheckCollisionTypeArrays();

	UFUNCTION()
		void GetActorScreenPosition(AActor* InActor, FVector2D& ScreenPosition, bool& ReturnValue);

	UFUNCTION()
		float GetDistanceToOwner(AActor* OtherActor);

	UFUNCTION()
		bool GetTargetRightSide(AActor* PotentialTarget);

	UFUNCTION()
		float CalculateDotProductToTarget(AActor* Target);

	UFUNCTION()
		void Initialize(UArrowComponent* InArrowComp);

	UFUNCTION()
		void DisableCameraLock();

	UFUNCTION()
		void FindTargetWithAxisInput(float AxisValue);

	UFUNCTION()
		void ToggleCameraLock();

	UFUNCTION()
		bool IsTargetingEnabled();

	UFUNCTION()
		void FindTargetOnLeft();

	UFUNCTION()
		void FindTargetOnRight();

	UFUNCTION()
		void SetFreeCamera(bool bFreeCamera);

};
