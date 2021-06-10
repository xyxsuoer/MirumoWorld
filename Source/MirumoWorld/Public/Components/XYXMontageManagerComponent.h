// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XYXMontageManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRUMOWORLD_API UXYXMontageManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXYXMontageManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable, Category = XYX)
		class	UAnimMontage* GetMontageForAction(EMontageAction Action, int32 Index);

	UFUNCTION(BlueprintCallable, Category = XYX)
		int32 GetMontageActionLastIndex(EMontageAction Action);

	UFUNCTION(BlueprintCallable, Category = XYX)
		int32 GetRandomMontageIndex(EMontageAction Action);

	UFUNCTION(BlueprintCallable, Category = XYX)
		EMontageAction GetLastRequestedAction() { return LastRequestedAction; }

private:

	UPROPERTY()
		EMontageAction LastRequestedAction;

};
