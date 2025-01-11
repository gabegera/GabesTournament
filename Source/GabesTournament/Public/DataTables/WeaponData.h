#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"

class AWeaponActor;
class AProjectileActor;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Slot0 = 0 UMETA(DisplayName = "Slot 0"),
	Slot1 = 1 UMETA(DisplayName = "Slot 1"),
	Slot2 = 2 UMETA(DisplayName = "Slot 2"),
	Slot3 = 3 UMETA(DisplayName = "Slot 3"),
	Slot4 = 4 UMETA(DisplayName = "Slot 4"),
	Slot5 = 5 UMETA(DisplayName = "Slot 5"),
	Slot6 = 6 UMETA(DisplayName = "Slot 6"),
	Slot7 = 7 UMETA(DisplayName = "Slot 7"),
	Slot8 = 8 UMETA(DisplayName = "Slot 8")
};

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Hitscan = 0 UMETA(DisplayName = "Hitscan"),
	Projectile = 1 UMETA(DisplayName = "Projectile"),
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	AssaultRifleBullets = 1 UMETA(DisplayName = "Assault Rifle Bullets"),
	AssaultRifleGrenades = 2 UMETA(DisplayName = "Assault Rifle Grenades"),
	BioRifle = 3 UMETA(DisplayName = "Bio Rifle Globs"),
	ShockRifle = 4 UMETA(DisplayName = "Shock Rifle Energy"),
	LinkGun = 5 UMETA(DisplayName = "Link Gun Plasma"),
	Minigun = 6 UMETA(DisplayName = "Minigun Bullets"),
	Flak = 7 UMETA(DisplayName = "Flak"),
	Rockets = 8 UMETA(DisplayName = "Rockets"),
	SniperRifle = 9 UMETA(Displayname = "Sniper Rifle Bullets"),
	Lightning = 10 UMETA(DisplayName = "Lightning Charge")
};

USTRUCT(BlueprintType)
struct GABESTOURNAMENT_API FWeaponData: public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FWeaponData &Other) const
	{
		return
		(
			DisplayName == Other.DisplayName &&
			AmmoType == Other.AmmoType &&
			ProjectileType == Other.ProjectileType &&
			WeaponSlot == Other.WeaponSlot
		);
	}

	bool IsNull() const
	{
		if (DisplayName.IsNone())
		{
			return true;
		}
		return false;
	}

	// ------ METADATA ------
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Metadata")
	FName DisplayName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Metadata")
	EWeaponSlot WeaponSlot = EWeaponSlot::Slot0;

	// ------ PROJECTILE VARIABLES ------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectiles / Ammo")
	EProjectileType ProjectileType = EProjectileType::Hitscan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectiles / Ammo",
	meta=(EditCondition = "ProjectileType == EProjectileType::Projectile", EditConditionHides))
	TSoftClassPtr<AProjectileActor> ProjectileActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectiles / Ammo")
	EAmmoType AmmoType = EAmmoType::AssaultRifleBullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectiles / Ammo")
	int32 StartingAmmoAmount = 20;

	// ------ SHOOTING VARIABLES ------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooting Variables")
	float Damage = 10.0f;

	// FireRate in Rounds Per Minute (RPM)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooting Variables")
	float FireRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooting Variables")
	float Range = 10000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooting Variables")
	float WeaponSpreadInDegrees = 0.0f;

	// ------ SHOTGUN VARIABLES ------
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shotgun Variables")
	bool IsShotgun = false;

	// How many projectiles are spawned when shooting a shotgun.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shotgun Variables",
	meta=(EditCondition = "IsShotgun", EditConditionHides, ClampMin = 1, UIMin = 1))
	int32 ShotgunProjectileCount = 8;

	
};

inline uint32 GetTypeHash(const FWeaponData& WeaponData)
{
	return FCrc::MemCrc32(&WeaponData, sizeof(WeaponData));
}