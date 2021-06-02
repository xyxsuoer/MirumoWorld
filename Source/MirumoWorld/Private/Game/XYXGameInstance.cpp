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
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_TwinDagger.Heroe_TwinDagger'"));
	MontageDataTables.Emplace(TEXT("Heroe_TwinDagger"), pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_OneHandSwordShield.Heroe_OneHandSwordShield'"));
	MontageDataTables.Emplace(TEXT("Heroe_OneHandSwordShield"), pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_OneHandAndShield.Heroe_OneHandAndShield'"));
	MontageDataTables.Emplace(TEXT("Heroe_OneHandAndShield"), pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_OneHandSword.Heroe_OneHandSword'"));
	MontageDataTables.Emplace(TEXT("Heroe_OneHandSword"), pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_GreatSword.Heroe_GreatSword'"));
	MontageDataTables.Emplace(TEXT("Heroe_GreatSword"), pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_TwoHandAxe.Heroe_TwoHandAxe'"));
	MontageDataTables.Emplace(TEXT("Heroe_TwoHandAxe"), pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_TwoHandHammer.Heroe_TwoHandHammer'"));
	MontageDataTables.Emplace(TEXT("Heroe_TwoHandHammer"), pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_TwinBlade.Heroe_TwinBlade'"));
	MontageDataTables.Emplace(TEXT("Heroe_TwinBlade"), pDataTable);
	pDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/Heroes/Heroe_Bow.Heroe_Bow'"));
	MontageDataTables.Emplace(TEXT("Heroe_Bow"), pDataTable);

	SoundDataTable = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/Mirumo/DataTables/SFX/SFX_Sounds.SFX_Sounds'"));
}

