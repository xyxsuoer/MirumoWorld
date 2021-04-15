// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/XYXData.h"
#include "XYXDisplayedItem.generated.h"

UCLASS()
class MIRUMOWORLD_API AXYXDisplayedItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXYXDisplayedItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable, Category = XYX)
		bool Attach();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = XYX)
		class UXYXEquipmentManagerComponent* EquipmentComp;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		int32 SlotIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = XYX)
		EItemType ItemType =  EItemType::ENone;

public:

	UFUNCTION()
		void SetEquipmemtComp(UXYXEquipmentManagerComponent* Comp);

	UFUNCTION()
		void SetSlotIndex(int32 Index);

	UFUNCTION()
		void SetItemType(EItemType Ty);

};
