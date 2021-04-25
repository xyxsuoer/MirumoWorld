// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/XYXGameInstance.h"


UXYXGameInstance::UXYXGameInstance()
{
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_Common.Heroe_Common'"));
	MontageDataTables.Emplace(TEXT("Heroe_Common"), pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_Unarmed.Heroe_Unarmed'"));
	MontageDataTables.Emplace(TEXT("Heroe_Unarmed") ,pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_DualSword.Heroe_DualSword'"));
	MontageDataTables.Emplace(TEXT("Heroe_DualSword"), pDataTable);
}

