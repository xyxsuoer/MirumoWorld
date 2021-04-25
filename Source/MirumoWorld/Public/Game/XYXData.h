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
	ENone													UMETA(DisplayName = "ENone"),									// 无
	EHealth												UMETA(DisplayName = "EHealth"),								// 生命值
	EStamina											UMETA(DisplayName = "EStamina"),								// 体力值 
	EMana													UMETA(DisplayName = "EMana"),									// 魔法值 
	EDamage												UMETA(DisplayName = "EDamage"),								// 物理伤害
	EMagicDamage									UMETA(DisplayName = "EMagicDamage"),						// 魔法伤害
	EArmor												UMETA(DisplayName = "EArmor"),									// 护甲
	ECritChance										UMETA(DisplayName = "ECritChance"),						// 暴击率
	ECritMultiplier								UMETA(DisplayName = "ECritMultiplier"),				// 暴击效果
	EAttackSpeed									UMETA(DisplayName = "EAttackSpeed"),						// 攻速
	EBlock												UMETA(DisplayName = "EBlock"),									// 格挡值
	EMeleeAttackStaminaCost				UMETA(DisplayName = "EMeleeAttackStaminaCost"),// 近战攻击消耗体力值
	ECastingSpeed									UMETA(DisplayName = "ECastingSpeed")						// 施法速度
};


UENUM(BlueprintType)
enum class EState : uint8
{
	EIdle													UMETA(DisplayName = "EIdle"),									// 闲着
	EDisabled											UMETA(DisplayName = "EDisabled"),							// 无法操作
	EAttacking										UMETA(DisplayName = "EAttacking"),							// 攻击中 
	ERolling											UMETA(DisplayName = "ERolling"),								// 翻滚中 
	EFalling											UMETA(DisplayName = "EFalling"),								// 下落中
	EInteracting									UMETA(DisplayName = "EInteracting"),						// 交互中
	EBackstabbing									UMETA(DisplayName = "EBackstabbing"),					// 背刺中
	EParrying											UMETA(DisplayName = "EParrying"),							// 招架中
	EDead													UMETA(DisplayName = "EDead")										// 死亡
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
	EIdle													UMETA(DisplayName = "ENone"),									// 闲着
	ECrouch												UMETA(DisplayName = "ECrouch"),								// 蹲着
	EWalk													UMETA(DisplayName = "EWalk"),									// 行走
	EJog													UMETA(DisplayName = "EJog"),										// 慢跑 
	ESprint												UMETA(DisplayName = "ESprint")									// 冲刺 
};


UENUM(BlueprintType)
enum class EMontageAction : uint8
{
	ENone													UMETA(DisplayName = "ENone"),									// 无
	ELightAttack									UMETA(DisplayName = "ELightAttack"),						// 轻击
	EHeavyAttack									UMETA(DisplayName = "EHeavyAttack"),						// 重击 
	EThrustAttack									UMETA(DisplayName = "EThrustAttack"),					// 刺出 
	ESpecialAttack								UMETA(DisplayName = "ESpecialAttack"),					// 特别攻击
	EFallingAttack								UMETA(DisplayName = "EFallingAttack"),					// 空中攻击
	EShootArrow										UMETA(DisplayName = "EShootArrow"),						// 射箭
	ERollForward									UMETA(DisplayName = "ERollForward"),						// 向前翻滚
	ERollBackward									UMETA(DisplayName = "ERollBackward"),					// 往后翻滚
	ERollLeft											UMETA(DisplayName = "ERollLeft"),							// 向左翻滚
	ERollRight										UMETA(DisplayName = "ERollRight"),							// 向右翻滚
	EDrawWeaopon									UMETA(DisplayName = "EDrawWeaopon"),						// 抽出武器
	EDisarmWeapon									UMETA(DisplayName = "EDisarmWeapon"),					// 收起武器
	EStunFront										UMETA(DisplayName = "EStunFront"),							// 击中前面 
	EStunBack											UMETA(DisplayName = "EStunBack"),							// 击中后面 
	EStunLeft											UMETA(DisplayName = "EStunLeft"),							// 击中左边 
	EStunRight										UMETA(DisplayName = "EStunRight"),							// 击中右边 
	EImpact												UMETA(DisplayName = "EImpact"),								// 撞击 
	EBlock												UMETA(DisplayName = "EBlock"),									// 格挡
	EParry												UMETA(DisplayName = "EParry"),									// 招架
	EParried											UMETA(DisplayName = "EParried"),								// 被招架
	EBackstab											UMETA(DisplayName = "EBackstab"),							// 背刺
	EBackstabbed									UMETA(DisplayName = "EBackstabbed"),						// 被背刺
	ESprint												UMETA(DisplayName = "ESprint"),								// 冲刺
	ECrouch												UMETA(DisplayName = "ECrouch")								// 下蹲
};


UENUM(BlueprintType)
enum class EMeleeAttackType : uint8
{
	ENone													UMETA(DisplayName = "ENone"),									// 无
	ELight												UMETA(DisplayName = "ELightAttack"),						// 轻击
	EHeavy												UMETA(DisplayName = "EHeavyAttack"),						// 重击 
	EThrust												UMETA(DisplayName = "EThrustAttack"),					// 刺出 
	ESpecial											UMETA(DisplayName = "ESpecialAttack"),					// 特别攻击
	EFalling											UMETA(DisplayName = "EFallingAttack")					// 空中攻击
};


UENUM(BlueprintType)
enum class EItemType : uint8
{
	ENone													UMETA(DisplayName = "ENone"),									// 无
	ESpell												UMETA(DisplayName = "ESpell"),								// 法术
	EShield												UMETA(DisplayName = "EShield"),								// 盾牌 
	EHead													UMETA(DisplayName = "EHead"),									// 头部 
	EBody													UMETA(DisplayName = "EBody"),									// 身体
	ELegs													UMETA(DisplayName = "ELegs"),									// 腿部
	EHands												UMETA(DisplayName = "EHands"),								// 手部
	EFeet													UMETA(DisplayName = "EFeet"),									// 足部
	ERing													UMETA(DisplayName = "ERing"),									// 戒指
	ENecklace											UMETA(DisplayName = "ENecklace"),							// 项链
	EArrows												UMETA(DisplayName = "EArrows"),								// 箭矢
	ETool													UMETA(DisplayName = "ETool"),									// 工具 
	EMaterial											UMETA(DisplayName = "EMaterial"),							// 材料 
	EMeleeWeaponRight							UMETA(DisplayName = "EMeleeWeaponRight"),			// 近战物品(物理)
	EMeleeWeaponLeft							UMETA(DisplayName = "EMeleeWeaponLeft"),			// 近战物品(物理)
	ERangeWeapon									UMETA(DisplayName = "ERangeWeapon")						// 远程武器(物理)
};


UENUM(BlueprintType)
enum class EInputBufferKey : uint8
{
	ENone													UMETA(DisplayName = "ENone"),									// 无
	ELightAttack									UMETA(DisplayName = "ELightAttack"),						// 轻击
	EHeavyAttack									UMETA(DisplayName = "EHeavyAttack"),						// 重击 
	EThrustAttack									UMETA(DisplayName = "EThrustAttack"),					// 刺出 
	ESpecialAttack								UMETA(DisplayName = "ESpecialAttack"),					// 特别攻击
	EFallingAttack								UMETA(DisplayName = "EFallingAttack"),					// 空中攻击
	ERoll													UMETA(DisplayName = "ERoll"),									// 翻滚
	EJump													UMETA(DisplayName = "EJump"),									// 跳
	EParry												UMETA(DisplayName = "EParry"),									// 招架
	EToggleCombat									UMETA(DisplayName = "EToggleCombat"),					// 切换战斗 
	ESwitchMainHandTypeUp					UMETA(DisplayName = "ESwitchMainHandTypeUp"),	// 切换上一个主手类型 
	ESwitchMainHandTypeDown				UMETA(DisplayName = "ESwitchMainHandTypeDown"),// 切换下一个主手类型
	ESwitchMainHandItemUp					UMETA(DisplayName = "ESwitchMainHandItemUp"),	// 切换上一个主手物品
	ESwitchMainHandItemDown				UMETA(DisplayName = "ESwitchMainHandItemDown"),// 切换下一个主手物品
	EAbilityAttack								UMETA(DisplayName = "EAbilityAttack"),					// 技能攻击 
	ESetSpellActiveIndex					UMETA(DisplayName = "ESetSpellActiveIndex")		// 设置激活的法术 
};


UENUM(BlueprintType)
enum class EHandleSameItemMethod : uint8
{
	EUnequip											UMETA(DisplayName = "EUnequip"),							// 卸下
	EUpdate												UMETA(DisplayName = "EUpdate")								// 更新
};


UENUM(BlueprintType)
enum class EEffectType : uint8
{
	ENone													UMETA(DisplayName = "ENone"),								// 无
	EStun													UMETA(DisplayName = "EStun"),								// 打昏
	EKnockdown										UMETA(DisplayName = "EKnockdown"),						// 击倒 
	EBurning											UMETA(DisplayName = "EBurning"),							// 燃烧 
	EBackstab											UMETA(DisplayName = "EBackstab"),						// 背刺
	EImpact												UMETA(DisplayName = "EImpact"),							// 撞击
	EParried											UMETA(DisplayName = "EParried")							// 被招架
};


UENUM(BlueprintType)
enum class EDirection : uint8
{
	EFront												UMETA(DisplayName = "EFront"),								// 前
	EBack													UMETA(DisplayName = "EBack"),								// 后
	ELeft													UMETA(DisplayName = "ELeft"),								// 左
	ERight												UMETA(DisplayName = "ERight")								// 右 
};


UENUM(BlueprintType)
enum class ECollisionPart : uint8
{
	ENone													UMETA(DisplayName = "ENone"),								// 无
	EMainHandItem									UMETA(DisplayName = "EMainHandItem	"),				// 主手物品
	ERightHand										UMETA(DisplayName = "ERightHand"),						// 右手  
	ELeftHand											UMETA(DisplayName = "ELeftHand"),						// 左手 
	ERightFoot										UMETA(DisplayName = "ERightFoot"),						// 右足
	ELeftFoot											UMETA(DisplayName = "ELeftFoot")							// 左足
};


UENUM(BlueprintType)
enum class EAttackResult : uint8
{
	ESuccess											UMETA(DisplayName = "EIdle"),								// 成功
	EFailed												UMETA(DisplayName = "EPatrol"),							// 失败
	EBlocked											UMETA(DisplayName = "EMeleeAttack"),					// 被格挡
	EParried											UMETA(DisplayName = "ERangeAttack")					// 被招架 
};


UENUM(BlueprintType)
enum class EApplyEffectMethod : uint8
{
	EStack												UMETA(DisplayName = "EStack"),								// 堆叠
	EReplace											UMETA(DisplayName = "EReplace")							// 替换
};


UENUM(BlueprintType)
enum class EAIBehavior : uint8
{
	EIdle													UMETA(DisplayName = "EIdle"),								// 空闲
	EPatrol												UMETA(DisplayName = "EPatrol"),							// 巡逻
	EMeleeAttack									UMETA(DisplayName = "EMeleeAttack"),					// 近战攻击  
	ERangeAttack									UMETA(DisplayName = "ERangeAttack"),					// 远程攻击 
	EApproach											UMETA(DisplayName = "EApproach"),						// 靠近中
	EFlee													UMETA(DisplayName = "EFlee"),								// 远离中
	EStrafeAround									UMETA(DisplayName = "EStrafeAround"),				// 周围徘徊
	EHit													UMETA(DisplayName = "EHit")									// 击打 
};


UENUM(BlueprintType)
enum class EActivity : uint8
{
	ENone													UMETA(DisplayName = "ENone"),								// 无
	EIsBlockingPressed						UMETA(DisplayName = "EIsBlockingPressed"),		// 格挡中
	EIsAmingPressed								UMETA(DisplayName = "EIsAmingPressed"),			// 瞄准中  
	EIsImmortal										UMETA(DisplayName = "EIsImmortal"),					// 无敌中 
	ECanBeInterrupted							UMETA(DisplayName = "ECanBeInterrupted"),		// 可被打断中 
	EIsLookingForward							UMETA(DisplayName = "EIsLookingForward"),		// 向前看中 
	ECanParryHit									UMETA(DisplayName = "ECanParryHit"),					// 可反击中 
	EIsZooming										UMETA(DisplayName = "EIsZooming")						// 放大中 
};


UENUM(BlueprintType)
enum class EAbilityEndResult : uint8
{
	EFinished											UMETA(DisplayName = "EFinished"),						// 完成
	EInterrupted									UMETA(DisplayName = "EInterrupted"),					// 打断
	ECanceled											UMETA(DisplayName = "ECanceled"),						// 取消  
	EOutOfMana										UMETA(DisplayName = "EOutOfMana"),						// 魔力不足 
	EDestroyed										UMETA(DisplayName = "EDestroyed")						// 销毁 
};


UENUM(BlueprintType)
enum class ETeam : uint8
{
	EEnemiesTeam									UMETA(DisplayName = "EEnemies Team"),				// 敌人
	EHeroesTeam										UMETA(DisplayName = "EHeroes Team"),					// 角色
	ENeutral											UMETA(DisplayName = "ENeutral")							// 中立      
};


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	ENone													UMETA(DisplayName = "ENone"),								// 徒手/盾
	EBow													UMETA(DisplayName = "EBow"),								// 弓
	ESpell												UMETA(DisplayName = "ESpell"),							// 法术
	EDualSwordLeft								UMETA(DisplayName = "EDualSwordLeft"),			// 双剑左手剑
	EDualSwordRight								UMETA(DisplayName = "EDualSwordRight"),			// 双剑右手剑
	EGreatSword										UMETA(DisplayName = "EGreatSword"),					// 巨剑
	EKatana												UMETA(DisplayName = "EKatana"),							// 太刀
	EOneHandAndShield							UMETA(DisplayName = "EOneHandAndShield"),		// 单手武器(钝器) + 盾
	EOneHandSwordShield						UMETA(DisplayName = "EOneHandSwordShield"),	// 单手剑 + 盾
	EOneHandSword									UMETA(DisplayName = "EOneHandSword"),				// 单手长剑/刀
	ESpear												UMETA(DisplayName = "ESpear"),							// 长矛
	ETwinBlade										UMETA(DisplayName = "ETwinBlade"),					// 长柄双刃(双剑/双刀合成)
	ETwinDaggerLeft								UMETA(DisplayName = "ETwinDaggerLeft"),					// 双匕首
	ETwinDaggerRight							UMETA(DisplayName = "ETwinDaggerRight"),					// 双匕首
	ETwoHandAxe										UMETA(DisplayName = "ETwoHandAxe"),					// 双手斧
	ETwoHandHammer								UMETA(DisplayName = "ETwoHandHammer"),			// 双手锤
	ETwoHandSword									UMETA(DisplayName = "ETwoHandSword")				// 双手剑
};


UENUM(BlueprintType)
enum class ECombatType : uint8
{
	EUnarmed											UMETA(DisplayName = "EUnarmed"),							// 徒手 
	EMelee												UMETA(DisplayName = "EMelee"),								// 近战 物理
	ERanged												UMETA(DisplayName = "ERange"),								// 远程 物理
	EMagic												UMETA(DisplayName = "EMagic")								// 魔法
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
