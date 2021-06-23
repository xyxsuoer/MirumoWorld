// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "XYXEQSContextStrafeLeft.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXEQSContextStrafeLeft : public UEnvQueryContext
{
	GENERATED_BODY()

public:

	UXYXEQSContextStrafeLeft();

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
