// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_BlueprintBase.h"
#include "XYXEQSContextStrafeTarget.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXEQSContextStrafeTarget : public UEnvQueryContext_BlueprintBase
{
	GENERATED_BODY()
	
public:

	UXYXEQSContextStrafeTarget();

	virtual UWorld* GetWorld() const override;

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

	void ProvideSingleActor(UObject* QuerierObject, AActor* QuerierActor, AActor*& ResultingActor) const;
};
