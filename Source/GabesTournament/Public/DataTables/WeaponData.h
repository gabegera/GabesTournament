#pragma once

#include "CoreMinimal.h"
#include "ShooterEnumLibrary.h"
#include "WeaponData.generated.h"

class AProjectile;

USTRUCT(BlueprintType)
struct GABESTOURNAMENT_API FWeaponData: public FTableRowBase
{
	GENERATED_BODY()	

	bool operator==(const FWeaponData &Other) const
	{
		if (DisplayName == Other.DisplayName &&
			WeaponSlot == Other.WeaponSlot &&
			SecondaryFireType == Other.SecondaryFireType &&
			PrimaryAmmoType == Other.PrimaryAmmoType &&
			SecondaryAmmoType == Other.SecondaryAmmoType &&
			PrimaryFireDamage == Other.PrimaryFireDamage &&
			SecondaryFireDamage == Other.SecondaryFireDamage &&
			PrimaryKnockbackForceMultiplier == Other.PrimaryKnockbackForceMultiplier &&
			SecondaryKnockbackForceMultiplier == Other.SecondaryKnockbackForceMultiplier &&
			PrimaryFireRate == Other.PrimaryFireRate &&
			SecondaryFireRate == Other.SecondaryFireRate &&
			isPrimaryFireAutomatic == Other.isPrimaryFireAutomatic &&
			isSecondaryFireAutomatic == Other.isSecondaryFireAutomatic &&
			PrimaryShotgunPelletCount == Other.PrimaryShotgunPelletCount &&
			SecondaryShotgunPelletCount == Other.SecondaryShotgunPelletCount &&
			PrimaryFireBulletSpread == Other.PrimaryFireBulletSpread &&
			SecondaryFireBulletSpread == Other.SecondaryFireBulletSpread &&
			PrimaryFireProjectile == Other.PrimaryFireProjectile &&
			SecondaryFireProjectile == Other.SecondaryFireProjectile &&
			DoesPrimaryOverheat == Other.DoesPrimaryOverheat &&
			OverheatThreshold == Other.OverheatThreshold &&
			CooldownDelay == Other.CooldownDelay &&
			CooldownSpeedMultiplier == Other.CooldownSpeedMultiplier &&
			DoesPrimaryCharge == Other.DoesPrimaryCharge &&
			PrimaryTimeToCharge == Other.PrimaryTimeToCharge
			)
		{
			return true;
		}
		
		return false;
	}

	bool operator!=(const FWeaponData &Other) const
	{
		return !(*this == Other);
	}

	UPROPERTY(EditAnywhere, Category="Weapon")
	FString DisplayName = "Weapon Name";

	UPROPERTY(EditAnywhere, Category="Weapon")
	EWeaponSlot WeaponSlot = EWeaponSlot::Slot1;

	UPROPERTY(EditAnywhere, Category="Weapon")
	ESecondaryFireType SecondaryFireType = ESecondaryFireType::Hitscan;

	UPROPERTY(EditAnywhere, Category="Ammo")
	EAmmoType PrimaryAmmoType = EAmmoType::None;
	
	// How much ammo is used up when the weapon is fired.
	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 PrimaryAmmoCost = 1;

	UPROPERTY(EditAnywhere, Category="Ammo")
	EAmmoType SecondaryAmmoType = EAmmoType::None;
	
	// How much ammo is used up when the weapon is fired.
	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 SecondaryAmmoCost = 1;

	// Sets whether the primary and secondary fire can be used at the same time.
	UPROPERTY(EditAnywhere, Category="Projectiles")
	bool CanShootPrimaryAndSecondarySimultaneously = true;

	// What projectile the Primary Fire shoots. Leave empty to shoot Hitscan.
	UPROPERTY(EditAnywhere, Category="Projectiles")
	TSoftClassPtr<AProjectile> PrimaryFireProjectile;

	// What projectile the Secondary Fire shoots. Leave empty to shoot Hitscan.
	UPROPERTY(EditAnywhere, Category="Projectiles")
	TSoftClassPtr<AProjectile> SecondaryFireProjectile;

	// Damage per projectile fired. If shotgun this will be the damage for every pellet/projectile fired.
	// If Projectile is Explosive, this will only affect Impact Damage.
	UPROPERTY(EditAnywhere, Category="Damage")
	float PrimaryFireDamage = 20;

	// Damage per projectile fired. If shotgun this will be the damage for every pellet/projectile fired.
	// If Projectile is Explosive, this will only affect Impact Damage.
	UPROPERTY(EditAnywhere, Category="Damage")
	float SecondaryFireDamage = 20;

	// How much force is applied to the target multiplied by the damage.
	// Only affect non-explosive projectiles.
	UPROPERTY(EditAnywhere, Category="Knockback")
	float PrimaryKnockbackForceMultiplier = 100.0f;

	// How much force is applied to the target multiplied by the damage.
	// Only affect non-explosive projectiles.
	UPROPERTY(EditAnywhere, Category="Knockback")
	float SecondaryKnockbackForceMultiplier = 100.0f;

	// If weapon is set to burst fire, this will be disabled.
	UPROPERTY(EditAnywhere, Category="Fire Rate", meta=(EditCondition="PrimaryBurstCount <= 1"))
	bool isPrimaryFireAutomatic = false;

	// If weapon is set to burst fire, this will be disabled.
	UPROPERTY(EditAnywhere, Category="Fire Rate", meta=(EditCondition="SecondaryBurstCount <= 1"))
	bool isSecondaryFireAutomatic = false;	

	// Fire Rate in RPM (Rounds per Minute)
	UPROPERTY(EditAnywhere, Category="Fire Rate")
	float PrimaryFireRate = 60;

	// Fire Rate in RPM (Rounds per Minute)
	UPROPERTY(EditAnywhere, Category="Fire Rate")
	float SecondaryFireRate = 60;

	// How many projectiles will be fired when the trigger is pulled.
	// Set to 1 to disable shotgun.
	UPROPERTY(EditAnywhere, Category="Shotgun", meta=(ClampMin="1"))
	int32 PrimaryShotgunPelletCount = 1;

	// How many projectiles will be fired when the trigger is pulled.
	// Set to 1 to disable shotgun.
	UPROPERTY(EditAnywhere, Category="Shotgun", meta=(ClampMin="1"))
	int32 SecondaryShotgunPelletCount = 1;

	// How many times the weapon will shoot consecutively.
	UPROPERTY(EditAnywhere, Category="Burst", meta=(ClampMin="1"))
	int32 PrimaryBurstCount = 1;

	// How many times the weapon will shoot consecutively.
	UPROPERTY(EditAnywhere, Category="Burst", meta=(ClampMin="1"))
	int32 SecondaryBurstCount = 1;

	UPROPERTY(EditAnywhere, Category="Spread", meta=(Units="Degrees"))
	float PrimaryFireBulletSpread = 0.0f;

	UPROPERTY(EditAnywhere, Category="Spread", meta=(Units="Degrees"))
	float SecondaryFireBulletSpread = 0.0f;

	// Does the weapon overheat if fired for extended period.
	UPROPERTY(EditAnywhere, Category="Overheating")
	bool DoesPrimaryOverheat = false;

	// Does the weapon overheat if fired for extended period.
	UPROPERTY(EditAnywhere, Category="Overheating")
	bool DoesSecondaryOverheat = false;

	// How long the weapon has to be fired before it overheats.
	UPROPERTY(EditAnywhere, Category="Overheating", meta=(Units="Seconds"))
	float OverheatThreshold = 10.0f;

	// How long the delay is before the weapon cools down.
	UPROPERTY(EditAnywhere, Category="Overheating", meta=(Units="Seconds"))
	float CooldownDelay = 0.2f;

	// How long the delay is before the weapon cools down when overheated.
	UPROPERTY(EditAnywhere, Category="Overheating", meta=(Units="Seconds"))
	float OverheatedCooldownDelay = 1.0f;

	// How fast the weapon cools down.
	UPROPERTY(EditAnywhere, Category="Overheating", meta=(ClampMin="0.1"))
	float CooldownSpeedMultiplier = 1.5f;

	// Does the weapon have to charge before firing.
	UPROPERTY(EditAnywhere, Category="Charging")
	bool DoesPrimaryCharge = false;

	// Does the weapon have to charge before firing.
	UPROPERTY(EditAnywhere, Category="Charging")
	bool DoesSecondaryCharge = false;

	// How Long the weapon takes to charge before it can be fired.
	UPROPERTY(EditAnywhere, Category="Charging", meta=(Units="Seconds", EditCondition="DoesPrimaryCharge"))
	float PrimaryTimeToCharge = 1.0f;

	// A multiplier to control how fast the weapon loses it's charge.
	UPROPERTY(EditAnywhere, Category="Charging", meta=(EditCondition="DoesPrimaryCharge"))
	float PrimaryDeChargeMultiplier = 1.0f;

	// How Long the weapon takes to charge before it can be fired.
	UPROPERTY(EditAnywhere, Category="Charging", meta=(Units="Seconds", EditCondition="DoesSecondaryCharge"))
	float SecondaryTimeToCharge = 1.0f;

	// A multiplier to control how fast the weapon loses it's charge.
	UPROPERTY(EditAnywhere, Category="Charging", meta=(EditCondition="DoesPrimaryCharge"))
	float SecondaryDeChargeMultiplier = 1.0f;
	
};