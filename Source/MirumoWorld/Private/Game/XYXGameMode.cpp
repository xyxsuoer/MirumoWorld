// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/XYXGameMode.h"
#include "Actors/XYXCharacter.h"
#include "UObject/ConstructorHelpers.h"

AXYXGameMode::AXYXGameMode()
{
	static ConstructorHelpers::FClassFinder<AXYXCharacter> BP_Heroe(TEXT("Blueprint'/Game/Mirumo/Blueprints/Heroes/BP_Heroe.BP_Heroe_C'"));
	if (BP_Heroe.Succeeded())
	{
		DefaultPawnClass = BP_Heroe.Class;
	}

}
