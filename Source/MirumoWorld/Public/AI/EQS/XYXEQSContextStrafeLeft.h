// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_BlueprintBase.h"
#include "XYXEQSContextStrafeLeft.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXEQSContextStrafeLeft : public UEnvQueryContext_BlueprintBase
{
	GENERATED_BODY()

public:

	UXYXEQSContextStrafeLeft();

	virtual UWorld* GetWorld() const override;

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

	void ProvideSingleLocation(UObject* QuerierObject, AActor* QuerierActor, FVector& ResultingLocation) const;

	
};
