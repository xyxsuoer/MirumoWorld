// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/XYXArrowProjectileBase.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Interfaces/XYXInterfaceEntity.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/ObjectItems/XYXItemArrow.h"

// Sets default values
AXYXArrowProjectileBase::AXYXArrowProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AXYXArrowProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AXYXArrowProjectileBase::ApplyHitImpulse(class UPrimitiveComponent* Comp, FVector HitNormal, FName BoneName)
{
	if (Comp && Comp->IsAnySimulatingPhysics())
	{
		FVector TmpImpulse = HitNormal * (-1.f) * ImpulsePower;
		Comp->AddImpulse(TmpImpulse, BoneName, false);
	}
}

void AXYXArrowProjectileBase::UpdateArrowMesh()
{
	auto OwnerObj = Cast<UXYXEquipmentManagerComponent>(GetOwner()->GetComponentByClass(UXYXEquipmentManagerComponent::StaticClass()));
	if (IsValid(OwnerObj))
	{
		auto TmpItem = OwnerObj->GetActiveItem(EItemType::EArrows, 0);
		TSubclassOf<class UXYXItemBase> ItemArrowClass = TmpItem.ItemClass;
		if (UKismetSystemLibrary::IsValidClass(ItemArrowClass))
		{
			auto ItemArrow = NewObject<UXYXItemArrow>(this, ItemArrowClass);
			if (ItemArrow)
			{
				StaticMesh->SetStaticMesh(ItemArrow->ArrowMesh);
			}
		}
	}
}

bool AXYXArrowProjectileBase::OnArrowHit(FHitResult Hit)
{
	return false;
}

bool AXYXArrowProjectileBase::IsEnemy(AActor* Target)
{
	IXYXInterfaceEntity* OwnerObj = Cast<IXYXInterfaceEntity>(GetOwner());
	IXYXInterfaceEntity* TargetObj = Cast<IXYXInterfaceEntity>(Target);
	if (OwnerObj && TargetObj)
	{
		ETeam TmpOwnerTeam = OwnerObj->GetEntityCombatTeam();
		ETeam TmpTargetTeam = TargetObj->GetEntityCombatTeam();
		if (TmpOwnerTeam != ETeam::ENeutral &&
			TmpTargetTeam != ETeam::ENeutral && 
			TmpOwnerTeam != TmpTargetTeam)
		{
			return true;
		}
	}

	return false;
}

void AXYXArrowProjectileBase::SpawnImpaledArrow(class USceneComponent* Comp, FName SocketName, AActor* InActor, FVector InLocation)
{

}

