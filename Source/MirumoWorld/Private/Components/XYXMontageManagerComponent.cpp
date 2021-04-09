// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XYXMontageManagerComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Interfaces/XYXInterfaceMontageManagerComp.h>
#include "Actors/XYXCharacter.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UXYXMontageManagerComponent::UXYXMontageManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UXYXMontageManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterOwner = Cast<AXYXCharacter>(GetOwner());
}

UAnimMontage* UXYXMontageManagerComponent::GetMontageForAction(EMontageAction Action, int32 Index)
{
	UAnimMontage* AnimMontage = nullptr;
	LastRequestedAction = Action;
	if (CharacterOwner)
	{
		UDataTable* pDataTable = CharacterOwner->GetMontages(Action);
		if (pDataTable)
		{
			UEnum* Enum = StaticEnum<EMontageAction>();
			if (Enum)
			{
				FString ActionStr = Enum->GetNameStringByIndex((int32)Action);
				if (!ActionStr.IsEmpty())
				{
					FName name = FName(*ActionStr);
					FMontageAction* pRow = pDataTable->FindRow<FMontageAction>(name, "");
					if (pRow && pRow->Montages.IsValidIndex(Index))
					{
						/*FString filePath = FString::FromInt(Index);
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *filePath));*/
						AnimMontage = pRow->Montages[Index];
					}
				}
			}
		}
	}

	return AnimMontage;
}

int32 UXYXMontageManagerComponent::GetMontageActionLastIndex(EMontageAction Action)
{
	int32 Index = -1;
	if (CharacterOwner)
	{
		UDataTable* pDataTable = CharacterOwner->GetMontages(Action);
		if (pDataTable)
		{
			UEnum* Enum = StaticEnum<EMontageAction>();
			if (Enum)
			{
				FString ActionStr = Enum->GetNameStringByIndex((int32)Action);
				if (!ActionStr.IsEmpty())
				{
					FName name = FName(*ActionStr);
					FMontageAction* pRow = pDataTable->FindRow<FMontageAction>(name, "");
					if (pRow)
					{
						Index = pRow->Montages.Num() - 1;
					}
				}
			}
		}
	}

	return Index;
}

int32 UXYXMontageManagerComponent::GetRandomMontageIndex(EMontageAction Action)
{
	int32 Index = GetMontageActionLastIndex(Action);
	return FMath::RandRange(0, Index);
}

