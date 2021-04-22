// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/XYXGameMode.h"
#include "Actors/XYXCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Components/XYXInventoryManagerComponent.h"
#include <Game/XYXSaveGame.h>


AXYXGameMode::AXYXGameMode()
{
	static ConstructorHelpers::FClassFinder<AXYXCharacter> BP_Heroe(TEXT("Blueprint'/Game/Mirumo/Blueprints/Heroes/BP_Heroe.BP_Heroe_C'"));
	if (BP_Heroe.Succeeded())
	{
		DefaultPawnClass = BP_Heroe.Class;
	}

	Character = Cast<AXYXCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AXYXGameMode::BeginPlay()
{
	Super::BeginPlay();

	UpdateInventoryValue();
	UpdateEquipmentValue();
	if (UGameplayStatics::DoesSaveGameExist(SaveGameName, 0))
	{
		LoadGame();
	}
	else
	{
		SaveGame();
	}
}

void AXYXGameMode::UpdateEquipmentValue()
{
	if (IsValid(Character))
	{
		UXYXEquipmentManagerComponent* EquipmentComp = Character->GetEquipmentManagerComponent();
		if (EquipmentComp)
		{
			EquipmentSlots = EquipmentComp->EquipmentSlots;
			bIsInCombat = EquipmentComp->bIsInCombat;
			SelectMainHandSlotType = EquipmentComp->GetSelectedMainHandType();
		}
	}
}

void AXYXGameMode::UpdateInventoryValue()
{
	if (IsValid(Character))
	{
		UXYXInventoryManagerComponent* InventoryComp = Character->GetInventoryManagerComponent();
		if (InventoryComp)
		{
			Inventory = InventoryComp->Inventory;
		}
	}
}

void AXYXGameMode::LoadGame()
{
	//	加载数据
	XYXSaveGame = Cast<UXYXSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameName, 0));
	//	修改数据
	EquipmentSlots = XYXSaveGame->EquipmentSlots;
	Inventory = XYXSaveGame->Inventory;
	bIsInCombat = XYXSaveGame->bIsInCombat;
	SelectMainHandSlotType = XYXSaveGame->SelectMainHandSlotType;
	StatValues = XYXSaveGame->StatValues;
	OnGameLoaded.Broadcast();
}

void AXYXGameMode::SaveGame()
{
	if (!IsValid(XYXSaveGame))
	{
		XYXSaveGame = Cast<UXYXSaveGame>(UGameplayStatics::CreateSaveGameObject(UXYXSaveGame::StaticClass()));
	}
	//	修改数据
	UpdateInventoryValue();
	UpdateEquipmentValue();
	XYXSaveGame->Inventory = Inventory;
	XYXSaveGame->EquipmentSlots = EquipmentSlots;
	XYXSaveGame->bIsInCombat = bIsInCombat;
	XYXSaveGame->SelectMainHandSlotType = SelectMainHandSlotType;
	XYXSaveGame->StatValues = StatValues;
	//	保存数据
	UGameplayStatics::SaveGameToSlot(XYXSaveGame, SaveGameName, 0);
	OnGameSaved.Broadcast();
}
