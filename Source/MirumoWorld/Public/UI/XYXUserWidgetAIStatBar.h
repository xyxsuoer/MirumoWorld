// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XYXUserWidgetAIStatBar.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXUserWidgetAIStatBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool  InitializeHealth(class UXYXExtendedStatComponent* Comp);
	virtual bool InitializeHealth_Implementation(class UXYXExtendedStatComponent* Comp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool  InitializeStamina(class UXYXExtendedStatComponent* Comp);
	virtual bool InitializeStamina_Implementation(class UXYXExtendedStatComponent* Comp);
};
