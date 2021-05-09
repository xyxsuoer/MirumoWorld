// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/XYXArrowProjectileBase.h"
#include "Components/XYXEquipmentManagerComponent.h"
#include "Interfaces/XYXInterfaceEntity.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/ObjectItems/XYXItemArrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Components/XYXCollisionHandlerComponent.h>
#include "Kismet/KismetMathLibrary.h"
#include "Actors/XYXImpaledArrow.h"

// Sets default values
AXYXArrowProjectileBase::AXYXArrowProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Instanced Static Mesh Comp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Comp"));
	ParticleSystem->SetupAttachment(StaticMeshComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Comp"));
	CollisionHandler = CreateDefaultSubobject<UXYXCollisionHandlerComponent>(TEXT("Collision Handler Comp"));
}

// Called when the game starts or when spawned
void AXYXArrowProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = GetActorForwardVector() * InitialSpeed;
	}

	UpdateArrowMesh();
	SetLifeSpan(LifeTime);
	
	if (CollisionHandler)
	{
		CollisionHandler->SetCollisionMesh(StaticMeshComp, StaticMeshComp->GetAllSocketNames());
		CollisionHandler->ActiveCollision(ECollisionPart::ENone);
		CollisionHandler->OnHit.AddDynamic(this, &AXYXArrowProjectileBase::HandleOnHit);
	}

	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([=]
		{
			ProjectileMovement->ProjectileGravityScale = 0.3f;
		}
	);

	UWorld* World = GetWorld();
	check(World);
	FTimerHandle TmpTimer;
	World->GetTimerManager().SetTimer(TmpTimer, TimerCallback, 0.3f, false);
	
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
			if (ItemArrow && StaticMeshComp)
			{
				StaticMeshComp->SetStaticMesh(ItemArrow->ArrowMesh);
			}
		}
	}
}

void AXYXArrowProjectileBase::OnArrowHit(FHitResult Hit)
{
	auto TmpHitActor = Hit.GetActor();
	auto TmpHitNormal = Hit.Normal;
	auto TmpHitLocation = Hit.Location;
	auto TmpHitComp = Hit.GetComponent();
	auto TmpHitBoneName = Hit.BoneName;

	UWorld* const World = GetWorld();
	check(World);

	if (TmpHitActor != GetOwner())
	{
		IXYXInterfaceEntity* TmpHitObj = Cast<IXYXInterfaceEntity>(TmpHitActor);
		if (TmpHitObj)
		{
			if (IsEnemy(TmpHitActor))
			{
				FName SocketName = TmpHitObj->GetHeadSocket();
				FVector HitFromDirection = UKismetMathLibrary::GetDirectionUnitVector(TmpHitLocation, TmpHitActor->GetActorLocation());
				float TmpDamage = SocketName == TmpHitBoneName ? Damage * HeadShotDamageMultiplier : Damage;
				FHitData TmpData;
				TmpData.CanBeBlocked = true;
				TmpData.CanBeParried = false;
				TmpData.CanReceivedImpact = false;
				TmpData.HitFromDirection = HitFromDirection;
				TmpData.Damage = TmpDamage;
				TmpData.DamageCauser = GetOwner();
				EAttackResult ResultType;
				bool Result = TmpHitObj->TakeDamage(TmpData, ResultType);
				if (ResultType != EAttackResult::EFailed)
				{
					if (Result)
					{
						// Play hit sound and Apply Stun Effect to hit actor
					}

					SpawnImpaledArrow(TmpHitComp, TmpHitBoneName, TmpHitActor, TmpHitLocation);
					ApplyHitImpulse(TmpHitComp, TmpHitNormal, TmpHitBoneName);
					World->DestroyActor(this);
				}
			}
		}
		else
		{
			SpawnImpaledArrow(TmpHitComp, TmpHitBoneName, TmpHitActor, TmpHitLocation);
			ApplyHitImpulse(TmpHitComp, TmpHitNormal, TmpHitBoneName);
			World->DestroyActor(this);
		}
	}
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
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		FTransform TmpTransform = UKismetMathLibrary::MakeTransform(InLocation, GetActorRotation(), GetActorScale3D());
		UWorld* const World = GetWorld();
		check(World);
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParams.Owner = InActor;
		ActorSpawnParams.Instigator = GetOwner()->GetInstigator();
		AXYXImpaledArrow* XYXActor = World->SpawnActor<AXYXImpaledArrow>(AXYXImpaledArrow::StaticClass(), TmpTransform, ActorSpawnParams);
		if (XYXActor)
		{
			XYXActor->SetArrowMesh(StaticMeshComp->GetStaticMesh());
			XYXActor->AttachToComponent(Comp, FAttachmentTransformRules::KeepWorldTransform, SocketName);
		}
	}
}

void AXYXArrowProjectileBase::HandleOnHit(FHitResult HitResult)
{
	OnArrowHit(HitResult);
}

