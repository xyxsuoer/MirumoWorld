// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XYXImpaledArrow.generated.h"

UCLASS()
class MIRUMOWORLD_API AXYXImpaledArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXYXImpaledArrow();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UStaticMeshComponent* ImpaledArrowMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		class UStaticMesh* ArrowMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XYX)
		float LifeTime = 15.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
		void SetArrowMesh(class UStaticMesh* Mesh);

	UFUNCTION()
	void OwnerDestroyed(AActor* DestroyedActor);

};
