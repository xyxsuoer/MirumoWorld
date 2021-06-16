// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/XYXANGetBackstabDmg.h"
#include "Components/XYXEffectsComponent.h"
#include <Interfaces/XYXInterfaceEntity.h>
#include "Game/XYXData.h"
#include "Game/XYXGameInstance.h"
#include "Game/XYXFunctionLibrary.h"

void UXYXANGetBackstabDmg::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		auto CharacterOwner = MeshComp->GetOwner();
		if (CharacterOwner)
		{
			auto EffectComp = CharacterOwner->FindComponentByClass<UXYXEffectsComponent>();
			if (EffectComp && EffectComp->IsEffectApplied(EEffectType::EBackstab))
			{
				FHitData HitData;
				HitData.CanBeBlocked = false;
				HitData.CanBeParried = false;
				HitData.CanReceivedImpact = false;
				HitData.HitFromDirection = FVector::ZeroVector;
				HitData.Damage = EffectComp->BackstabDamage;
				HitData.DamageCauser = EffectComp->GetEffectApplier(EEffectType::EBackstab);

				IXYXInterfaceEntity* TmpActor = Cast<IXYXInterfaceEntity>(CharacterOwner);
				if (TmpActor)
				{
					EAttackResult ResultType;
					bool CanAttacked = TmpActor->Execute_TakeAttackDamage(CharacterOwner, HitData, ResultType);
					if (CanAttacked)
					{
						UWorld* World = CharacterOwner->GetWorld();
						check(World);
						UXYXGameInstance* GameInstance = Cast<UXYXGameInstance>(World->GetGameInstance());
						UXYXFunctionLibrary::PlayHitSound(GameInstance, HitData.DamageCauser, CharacterOwner, CharacterOwner->GetActorLocation());
					}
				}
			}
		}
	}
}

