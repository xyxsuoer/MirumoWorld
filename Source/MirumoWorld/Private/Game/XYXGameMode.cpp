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
}

void AXYXGameMode::BeginPlay()
{
	Super::BeginPlay();

	UpdateInventoryValue();
	UpdateEquipmentValue();

	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().SetTimer(LoadSaveTimer, this, &AXYXGameMode::LoadOrSave, 0.001f, false);

}

void AXYXGameMode::UpdateEquipmentValue()
{
	AXYXCharacter* Character = Cast<AXYXCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Character))
	{
		UXYXEquipmentManagerComponent* EquipmentComp = Character->GetEquipmentManagerComponent();
		if (EquipmentComp)
		{
			EquipmentSlots = EquipmentComp->EquipmentSlots;
			bIsInCombat = EquipmentComp->bIsInCombat;
			SelectMainHandSlotType = EquipmentComp->GetSelectedMainHandType();
			SelectMainHandSlotIndex = EquipmentComp->GetSelectedMainHandSlotIndex();
		}
	}
}

void AXYXGameMode::UpdateInventoryValue()
{
	AXYXCharacter* Character = Cast<AXYXCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Character))
	{
		UXYXInventoryManagerComponent* InventoryComp = Character->GetInventoryManagerComponent();
		if (InventoryComp)
		{
			Inventory = InventoryComp->Inventory;
		}
	}
}

void AXYXGameMode::LoadOrSave()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameName, 0))
	{
		LoadGame();
	}
	else
	{
		SaveGame();
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
	SelectMainHandSlotIndex = XYXSaveGame->SelectMainHandSlotIndex;
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
	XYXSaveGame->SelectMainHandSlotIndex = SelectMainHandSlotIndex;
	XYXSaveGame->StatValues = StatValues;
	//	保存数据
	UGameplayStatics::SaveGameToSlot(XYXSaveGame, SaveGameName, 0);
	OnGameSaved.Broadcast();
}
