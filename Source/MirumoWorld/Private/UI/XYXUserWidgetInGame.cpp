// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/XYXUserWidgetInGame.h"


bool UXYXUserWidgetInGame::ShowCrosshair_Implementation(UTexture2D* InTexture)
{
	return true;
}

bool UXYXUserWidgetInGame::HideCrosshair_Implementation()
{
	return false;
}

bool UXYXUserWidgetInGame::UpdateCrosshairPosition_Implementation(FVector InWorldPosition)
{
	return false;
}
