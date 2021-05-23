// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XYXUserWidgetInGame.generated.h"

/**
 * 
 */
UCLASS()
class MIRUMOWORLD_API UXYXUserWidgetInGame : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool ShowCrosshair(UTexture2D* InTexture);
	virtual bool ShowCrosshair_Implementation(UTexture2D* InTexture);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool HideCrosshair();
	virtual bool HideCrosshair_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = XYX)
		bool UpdateCrosshairPosition(FVector InWorldPosition);
	virtual bool UpdateCrosshairPosition_Implementation(FVector InWorldPosition);
	
};
