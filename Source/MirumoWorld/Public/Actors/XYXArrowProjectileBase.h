// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XYXArrowProjectileBase.generated.h"

UCLASS()
class MIRUMOWORLD_API AXYXArrowProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AXYXArrowProjectileBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float Damage = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float HeadShotDamageMultiplier = 2.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float InitialSpeed = 7000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float LifeTime = 15.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float ImpulsePower = 20000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UXYXCollisionHandlerComponent* CollisionHandler;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		TSubclassOf<class AXYXImpaledArrow> ImpledArrowClass;

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void ApplyHitImpulse(class UPrimitiveComponent* Comp, FVector HitNormal, FName BoneName);

	UFUNCTION()
		void UpdateArrowMesh();

	UFUNCTION()
		void OnArrowHit(FHitResult Hit);

	UFUNCTION()
		bool IsEnemy(AActor* Target);

	UFUNCTION()
		void SpawnImpaledArrow(class USceneComponent* Comp, FName SocketName, AActor* InActor, FVector InLocation);

	UFUNCTION()
		void HandleOnHit(FHitResult HitResult);
};
