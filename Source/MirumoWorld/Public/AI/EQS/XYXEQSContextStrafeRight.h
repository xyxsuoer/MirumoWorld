// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "XYXEQSContextStrafeRight.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXEQSContextStrafeRight : public UEnvQueryContext
{
	GENERATED_BODY()

public:

	UXYXEQSContextStrafeRight();

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
