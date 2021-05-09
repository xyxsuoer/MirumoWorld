// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "XYXCollisionHandlerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, FHitResult, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollisionActivated, ECollisionPart, CollisionPart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionDeactivated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXCollisionHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXCollisionHandlerComponent();

	UPROPERTY(BlueprintAssignable)
		FOnHit OnHit;

	UPROPERTY(BlueprintAssignable)
		FOnCollisionActivated OnCollisionActivated;

	UPROPERTY(BlueprintAssignable)
		FOnCollisionDeactivated OnCollisionDeactivated;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		class UPrimitiveComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<FName> Sockets = {};

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bCanPerformTrace = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		bool bCollisionActive = false;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TMap<FName, FVector> LastSocketLocations = {};

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		TArray<AActor*> ActorsHit = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToCollideWith = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TArray<FName> IgnoredCollisionProfileNames = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TEnumAsByte <EDrawDebugTrace::Type> VisualizeTrace;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TArray<TSubclassOf<AActor>> IgnoreClasses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float TraceRadius = 0.1f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION()
		bool IsIgnoredClass(TSubclassOf<AActor> TestClass);

	UFUNCTION()
		bool IsCollisionActive();

	UFUNCTION()
		void SetCollisionMesh(class UPrimitiveComponent* WeaponMesh, TArray<FName> InSockets);

	UFUNCTION()
		void ActiveCollision(ECollisionPart CollisionPart);

	UFUNCTION()
		void DeactiveCollision();

	UFUNCTION()
		void UpdateLastSocketPosition();

	UFUNCTION()
		void PerformTrace();
};
