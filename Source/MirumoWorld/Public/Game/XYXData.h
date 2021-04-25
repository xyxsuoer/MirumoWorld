// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/AnimMontage.h"
#include "Engine/DataTable.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/Guid.h"
#include "XYXData.generated.h"



 //Enumerations
UENUM(BlueprintType)
enum class EKatanaStance : uint8
{
	EKatanaS1											UMETA(DisplayName = "EKatanaS1"),
	EKatanaS2											UMETA(DisplayName = "EKatanaS2"),
	EKatanaS3											UMETA(DisplayName = "EKatanaS3")
};


UENUM(BlueprintType)
enum class EAttributesType : uint8
{
	ENone													UMETA(DisplayName = "ENone"),									// ��
	EHealth												UMETA(DisplayName = "EHealth"),								// ����ֵ
	EStamina											UMETA(DisplayName = "EStamina"),								// ����ֵ 
	EMana													UMETA(DisplayName = "EMana"),									// ħ��ֵ 
	EDamage												UMETA(DisplayName = "EDamage"),								// �����˺�
	EMagicDamage									UMETA(DisplayName = "EMagicDamage"),						// ħ���˺�
	EArmor												UMETA(DisplayName = "EArmor"),									// ����
	ECritChance										UMETA(DisplayName = "ECritChance"),						// ������
	ECritMultiplier								UMETA(DisplayName = "ECritMultiplier"),				// ����Ч��
	EAttackSpeed									UMETA(DisplayName = "EAttackSpeed"),						// ����
	EBlock												UMETA(DisplayName = "EBlock"),									// ��ֵ
	EMeleeAttackStaminaCost				UMETA(DisplayName = "EMeleeAttackStaminaCost"),// ��ս������������ֵ
	ECastingSpeed									UMETA(DisplayName = "ECastingSpeed")						// ʩ���ٶ�
};


UENUM(BlueprintType)
enum class EState : uint8
{
	EIdle													UMETA(DisplayName = "EIdle"),									// ����
	EDisabled											UMETA(DisplayName = "EDisabled"),							// �޷�����
	EAttacking										UMETA(DisplayName = "EAttacking"),							// ������ 
	ERolling											UMETA(DisplayName = "ERolling"),								// ������ 
	EFalling											UMETA(DisplayName = "EFalling"),								// ������
	EInteracting									UMETA(DisplayName = "EInteracting"),						// ������
	EBackstabbing									UMETA(DisplayName = "EBackstabbing"),					// ������
	EParrying											UMETA(DisplayName = "EParrying"),							// �м���
	EDead													UMETA(DisplayName = "EDead")										// ����
};


UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	EFaceCamera										UMETA(DisplayName = "EFaceCamera"),
	EOrientToMovement							UMETA(DisplayName = "EOrientToMovement")
};


UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EIdle													UMETA(DisplayName = "ENone"),									// ����
	ECrouch												UMETA(DisplayName = "ECrouch"),								// ����
	EWalk													UMETA(DisplayName = "EWalk"),									// ����
	EJog													UMETA(DisplayName = "EJog"),										// ���� 
	ESprint												UMETA(DisplayName = "ESprint")									// ��� 
};


UENUM(BlueprintType)
enum class EMontageAction : uint8
{
	ENone													UMETA(DisplayName = "ENone"),									// ��
	ELightAttack									UMETA(DisplayName = "ELightAttack"),						// ���
	EHeavyAttack									UMETA(DisplayName = "EHeavyAttack"),						// �ػ� 
	EThrustAttack									UMETA(DisplayName = "EThrustAttack"),					// �̳� 
	ESpecialAttack								UMETA(DisplayName = "ESpecialAttack"),					// �ر𹥻�
	EFallingAttack								UMETA(DisplayName = "EFallingAttack"),					// ���й���
	EShootArrow										UMETA(DisplayName = "EShootArrow"),						// ���
	ERollForward									UMETA(DisplayName = "ERollForward"),						// ��ǰ����
	ERollBackward									UMETA(DisplayName = "ERollBackward"),					// ���󷭹�
	ERollLeft											UMETA(DisplayName = "ERollLeft"),							// ���󷭹�
	ERollRight										UMETA(DisplayName = "ERollRight"),							// ���ҷ���
	EDrawWeaopon									UMETA(DisplayName = "EDrawWeaopon"),						// �������
	EDisarmWeapon									UMETA(DisplayName = "EDisarmWeapon"),					// ��������
	EStunFront										UMETA(DisplayName = "EStunFront"),							// ����ǰ�� 
	EStunBack											UMETA(DisplayName = "EStunBack"),							// ���к��� 
	EStunLeft											UMETA(DisplayName = "EStunLeft"),							// ������� 
	EStunRight										UMETA(DisplayName = "EStunRight"),							// �����ұ� 
	EImpact												UMETA(DisplayName = "EImpact"),								// ײ�� 
	EBlock												UMETA(DisplayName = "EBlock"),									// ��
	EParry												UMETA(DisplayName = "EParry"),									// �м�
	EParried											UMETA(DisplayName = "EParried"),								// ���м�
	EBackstab											UMETA(DisplayName = "EBackstab"),							// ����
	EBackstabbed									UMETA(DisplayName = "EBackstabbed"),						// ������
	ESprint												UMETA(DisplayName = "ESprint"),								// ���
	ECrouch												UMETA(DisplayName = "ECrouch")								// �¶�
};


UENUM(BlueprintType)
enum class EMeleeAttackType : uint8
{
	ENone													UMETA(DisplayName = "ENone"),									// ��
	ELight												UMETA(DisplayName = "ELightAttack"),						// ���
	EHeavy												UMETA(DisplayName = "EHeavyAttack"),						// �ػ� 
	EThrust												UMETA(DisplayName = "EThrustAttack"),					// �̳� 
	ESpecial											UMETA(DisplayName = "ESpecialAttack"),					// �ر𹥻�
	EFalling											UMETA(DisplayName = "EFallingAttack")					// ���й���
};


UENUM(BlueprintType)
enum class EItemType : uint8
{
	ENone													UMETA(DisplayName = "ENone"),									// ��
	ESpell												UMETA(DisplayName = "ESpell"),								// ����
	EShield												UMETA(DisplayName = "EShield"),								// ���� 
	EHead													UMETA(DisplayName = "EHead"),									// ͷ�� 
	EBody													UMETA(DisplayName = "EBody"),									// ����
	ELegs													UMETA(DisplayName = "ELegs"),									// �Ȳ�
	EHands												UMETA(DisplayName = "EHands"),								// �ֲ�
	EFeet													UMETA(DisplayName = "EFeet"),									// �㲿
	ERing													UMETA(DisplayName = "ERing"),									// ��ָ
	ENecklace											UMETA(DisplayName = "ENecklace"),							// ����
	EArrows												UMETA(DisplayName = "EArrows"),								// ��ʸ
	ETool													UMETA(DisplayName = "ETool"),									// ���� 
	EMaterial											UMETA(DisplayName = "EMaterial"),							// ���� 
	EMeleeWeaponRight							UMETA(DisplayName = "EMeleeWeaponRight"),			// ��ս��Ʒ(����)
	EMeleeWeaponLeft							UMETA(DisplayName = "EMeleeWeaponLeft"),			// ��ս��Ʒ(����)
	ERangeWeapon									UMETA(DisplayName = "ERangeWeapon")						// Զ������(����)
};


UENUM(BlueprintType)
enum class EInputBufferKey : uint8
{
	ENone													UMETA(DisplayName = "ENone"),									// ��
	ELightAttack									UMETA(DisplayName = "ELightAttack"),						// ���
	EHeavyAttack									UMETA(DisplayName = "EHeavyAttack"),						// �ػ� 
	EThrustAttack									UMETA(DisplayName = "EThrustAttack"),					// �̳� 
	ESpecialAttack								UMETA(DisplayName = "ESpecialAttack"),					// �ر𹥻�
	EFallingAttack								UMETA(DisplayName = "EFallingAttack"),					// ���й���
	ERoll													UMETA(DisplayName = "ERoll"),									// ����
	EJump													UMETA(DisplayName = "EJump"),									// ��
	EParry												UMETA(DisplayName = "EParry"),									// �м�
	EToggleCombat									UMETA(DisplayName = "EToggleCombat"),					// �л�ս�� 
	ESwitchMainHandTypeUp					UMETA(DisplayName = "ESwitchMainHandTypeUp"),	// �л���һ���������� 
	ESwitchMainHandTypeDown				UMETA(DisplayName = "ESwitchMainHandTypeDown"),// �л���һ����������
	ESwitchMainHandItemUp					UMETA(DisplayName = "ESwitchMainHandItemUp"),	// �л���һ��������Ʒ
	ESwitchMainHandItemDown				UMETA(DisplayName = "ESwitchMainHandItemDown"),// �л���һ��������Ʒ
	EAbilityAttack								UMETA(DisplayName = "EAbilityAttack"),					// ���ܹ��� 
	ESetSpellActiveIndex					UMETA(DisplayName = "ESetSpellActiveIndex")		// ���ü���ķ��� 
};


UENUM(BlueprintType)
enum class EHandleSameItemMethod : uint8
{
	EUnequip											UMETA(DisplayName = "EUnequip"),							// ж��
	EUpdate												UMETA(DisplayName = "EUpdate")								// ����
};


UENUM(BlueprintType)
enum class EEffectType : uint8
{
	ENone													UMETA(DisplayName = "ENone"),								// ��
	EStun													UMETA(DisplayName = "EStun"),								// ���
	EKnockdown										UMETA(DisplayName = "EKnockdown"),						// ���� 
	EBurning											UMETA(DisplayName = "EBurning"),							// ȼ�� 
	EBackstab											UMETA(DisplayName = "EBackstab"),						// ����
	EImpact												UMETA(DisplayName = "EImpact"),							// ײ��
	EParried											UMETA(DisplayName = "EParried")							// ���м�
};


UENUM(BlueprintType)
enum class EDirection : uint8
{
	EFront												UMETA(DisplayName = "EFront"),								// ǰ
	EBack													UMETA(DisplayName = "EBack"),								// ��
	ELeft													UMETA(DisplayName = "ELeft"),								// ��
	ERight												UMETA(DisplayName = "ERight")								// �� 
};


UENUM(BlueprintType)
enum class ECollisionPart : uint8
{
	ENone													UMETA(DisplayName = "ENone"),								// ��
	EMainHandItem									UMETA(DisplayName = "EMainHandItem	"),				// ������Ʒ
	ERightHand										UMETA(DisplayName = "ERightHand"),						// ����  
	ELeftHand											UMETA(DisplayName = "ELeftHand"),						// ���� 
	ERightFoot										UMETA(DisplayName = "ERightFoot"),						// ����
	ELeftFoot											UMETA(DisplayName = "ELeftFoot")							// ����
};


UENUM(BlueprintType)
enum class EAttackResult : uint8
{
	ESuccess											UMETA(DisplayName = "EIdle"),								// �ɹ�
	EFailed												UMETA(DisplayName = "EPatrol"),							// ʧ��
	EBlocked											UMETA(DisplayName = "EMeleeAttack"),					// ����
	EParried											UMETA(DisplayName = "ERangeAttack")					// ���м� 
};


UENUM(BlueprintType)
enum class EApplyEffectMethod : uint8
{
	EStack												UMETA(DisplayName = "EStack"),								// �ѵ�
	EReplace											UMETA(DisplayName = "EReplace")							// �滻
};


UENUM(BlueprintType)
enum class EAIBehavior : uint8
{
	EIdle													UMETA(DisplayName = "EIdle"),								// ����
	EPatrol												UMETA(DisplayName = "EPatrol"),							// Ѳ��
	EMeleeAttack									UMETA(DisplayName = "EMeleeAttack"),					// ��ս����  
	ERangeAttack									UMETA(DisplayName = "ERangeAttack"),					// Զ�̹��� 
	EApproach											UMETA(DisplayName = "EApproach"),						// ������
	EFlee													UMETA(DisplayName = "EFlee"),								// Զ����
	EStrafeAround									UMETA(DisplayName = "EStrafeAround"),				// ��Χ�ǻ�
	EHit													UMETA(DisplayName = "EHit")									// ���� 
};


UENUM(BlueprintType)
enum class EActivity : uint8
{
	ENone													UMETA(DisplayName = "ENone"),								// ��
	EIsBlockingPressed						UMETA(DisplayName = "EIsBlockingPressed"),		// ����
	EIsAmingPressed								UMETA(DisplayName = "EIsAmingPressed"),			// ��׼��  
	EIsImmortal										UMETA(DisplayName = "EIsImmortal"),					// �޵��� 
	ECanBeInterrupted							UMETA(DisplayName = "ECanBeInterrupted"),		// �ɱ������ 
	EIsLookingForward							UMETA(DisplayName = "EIsLookingForward"),		// ��ǰ���� 
	ECanParryHit									UMETA(DisplayName = "ECanParryHit"),					// �ɷ����� 
	EIsZooming										UMETA(DisplayName = "EIsZooming")						// �Ŵ��� 
};


UENUM(BlueprintType)
enum class EAbilityEndResult : uint8
{
	EFinished											UMETA(DisplayName = "EFinished"),						// ���
	EInterrupted									UMETA(DisplayName = "EInterrupted"),					// ���
	ECanceled											UMETA(DisplayName = "ECanceled"),						// ȡ��  
	EOutOfMana										UMETA(DisplayName = "EOutOfMana"),						// ħ������ 
	EDestroyed										UMETA(DisplayName = "EDestroyed")						// ���� 
};


UENUM(BlueprintType)
enum class ETeam : uint8
{
	EEnemiesTeam									UMETA(DisplayName = "EEnemies Team"),				// ����
	EHeroesTeam										UMETA(DisplayName = "EHeroes Team"),					// ��ɫ
	ENeutral											UMETA(DisplayName = "ENeutral")							// ����      
};


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	ENone													UMETA(DisplayName = "ENone"),								// ͽ��/��
	EBow													UMETA(DisplayName = "EBow"),								// ��
	ESpell												UMETA(DisplayName = "ESpell"),							// ����
	EDualSwordLeft								UMETA(DisplayName = "EDualSwordLeft"),			// ˫�����ֽ�
	EDualSwordRight								UMETA(DisplayName = "EDualSwordRight"),			// ˫�����ֽ�
	EGreatSword										UMETA(DisplayName = "EGreatSword"),					// �޽�
	EKatana												UMETA(DisplayName = "EKatana"),							// ̫��
	EOneHandAndShield							UMETA(DisplayName = "EOneHandAndShield"),		// ��������(����) + ��
	EOneHandSwordShield						UMETA(DisplayName = "EOneHandSwordShield"),	// ���ֽ� + ��
	EOneHandSword									UMETA(DisplayName = "EOneHandSword"),				// ���ֳ���/��
	ESpear												UMETA(DisplayName = "ESpear"),							// ��ì
	ETwinBlade										UMETA(DisplayName = "ETwinBlade"),					// ����˫��(˫��/˫���ϳ�)
	ETwinDaggerLeft								UMETA(DisplayName = "ETwinDaggerLeft"),					// ˫ذ��
	ETwinDaggerRight							UMETA(DisplayName = "ETwinDaggerRight"),					// ˫ذ��
	ETwoHandAxe										UMETA(DisplayName = "ETwoHandAxe"),					// ˫�ָ�
	ETwoHandHammer								UMETA(DisplayName = "ETwoHandHammer"),			// ˫�ִ�
	ETwoHandSword									UMETA(DisplayName = "ETwoHandSword")				// ˫�ֽ�
};


UENUM(BlueprintType)
enum class ECombatType : uint8
{
	EUnarmed											UMETA(DisplayName = "EUnarmed"),							// ͽ�� 
	EMelee												UMETA(DisplayName = "EMelee"),								// ��ս ����
	ERanged												UMETA(DisplayName = "ERange"),								// Զ�� ����
	EMagic												UMETA(DisplayName = "EMagic")								// ħ��
};


// Structs

USTRUCT(BlueprintType)
struct FCameraSettings
{
	GENERATED_BODY()

public:

	FCameraSettings() {};

	FCameraSettings(FRotator rotation, float armlength, FVector socketoffset, float cameralagspeed)
	{
		Rotation = rotation;
		ArmLength = armlength;
		SocketOffset = socketoffset;
		CameraLagSpeed = cameralagspeed;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float ArmLength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FVector SocketOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float CameraLagSpeed = 0.f;

};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EItemType Type = EItemType::ENone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bIsDroppable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bIsConsumable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		UTexture2D* Image;

};

USTRUCT(BlueprintType)
struct FAttributes 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EAttributesType Type = EAttributesType::ENone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float BaseValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float ModifiersValue = 0.f;

};

USTRUCT(BlueprintType)
struct FModifier 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EAttributesType Type = EAttributesType::ENone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float Value = 0.f;

};

USTRUCT(BlueprintType)
struct FHitData 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		AActor* DamageCauser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FVector HitFromDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool CanBeParried = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool CanBeBlocked = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool CanReceivedImpact = true;

};

USTRUCT(BlueprintType)
struct FDisplayedItems 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		TArray<class AXYXDisplayedItem*>  DisplayedItems;

};

USTRUCT(BlueprintType)
struct FStoredItem 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		FGuid Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		TSubclassOf<class UXYXItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		int32 Amount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EWeaponType ItemWeaponType = EWeaponType::ENone;

};

USTRUCT(BlueprintType)
struct FDissolvedComponent 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		UPrimitiveComponent* Component;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float Value = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		TArray <UMaterialInterface*> Materials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		TArray<UMaterialInstanceDynamic*>  DissolvedMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bReverse = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool bIsRunning = false;

};

USTRUCT(BlueprintType)
struct FEquipmentSlot 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		TArray<FStoredItem>  Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		int32  ActiveItemIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		bool  bIsHidden = false;

};


USTRUCT(BlueprintType)
struct FEquipmentSlots 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EItemType  Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		TArray<FEquipmentSlot> Slots;
};



USTRUCT(BlueprintType)
struct FEffect 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EEffectType Type = EEffectType::EStun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		float Duration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		AActor* Applier;

};

USTRUCT(BlueprintType)
struct FMontageAction : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		EMontageAction Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XYX)
		TArray<UAnimMontage*>  Montages;

};



UCLASS()
class MIRUMOWORLD_API UXYXData : public UObject
{
	GENERATED_BODY()
	
};
