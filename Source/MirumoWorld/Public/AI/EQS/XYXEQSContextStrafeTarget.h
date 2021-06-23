// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "XYXEQSContextStrafeTarget.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXEQSContextStrafeTarget : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:

	UXYXEQSContextStrafeTarget();

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
