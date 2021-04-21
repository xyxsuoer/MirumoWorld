// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DisplayedItems/XYXDisplayedItemHand.h"

AXYXDisplayedItemHand::AXYXDisplayedItemHand()
{

}

void AXYXDisplayedItemHand::BeginPlay()
{

}

FName AXYXDisplayedItemHand::GetAttachmentSocket()
{
	Super::GetAttachmentSocket();
	return HandAttachmentSocket;
}
