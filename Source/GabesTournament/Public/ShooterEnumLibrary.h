#pragma once

#include "CoreMinimal.h"
#include "ShooterEnumLibrary.generated.h"

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Slot1 = 0 UMETA(DisplayName = "Slot 1"),
	Slot2 = 1 UMETA(DisplayName = "Slot 2"),
	Slot3 = 2 UMETA(DisplayName = "Slot 3"),
	Slot4 = 3 UMETA(DisplayName = "Slot 4"),
	Slot5 = 4 UMETA(DisplayName = "Slot 5"),
	Slot6 = 5 UMETA(DisplayName = "Slot 6"),
	Slot7 = 6 UMETA(DisplayName = "Slot 7"),
	Slot8 = 7 UMETA(DisplayName = "Slot 8"),
	Slot9 = 8 UMETA(DisplayName = "Slot 9")
};

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Hitscan = 0 UMETA(DisplayName = "Hitscan"),
	Projectile = 1 UMETA(DisplayName = "Projectile"),
	Custom = 2 UMETA(DisplayName = "Custom"),
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

UENUM(BlueprintType)
enum class ESecondaryFireType : uint8
{
	Hitscan = 0 UMETA(DisplayName = "Hitscan"),
	Projectile = 1 UMETA(DisplayName = "Projectile"),
	Zoom = 2 UMETA(DisplayName = "Zoom"),
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
	BlueTeam = 0 UMETA(DisplayName = "Blue Team"),
	RedTeam = 1 UMETA(DisplayName = "Red Team"),
	OrangeTeam = 2 UMETA(DisplayName = "Orange Team"),
	YellowTeam = 3 UMETA(DisplayName = "Yellow Team"),
	GreenTeam = 4 UMETA(DisplayName = "Green Team"),
	PurpleTeam = 5 UMETA(DisplayName = "Purple Team"),
	CyanTeam = 6 UMETA(DisplayName = "Cyan Team"),
	SilverTeam = 7 UMETA(DisplayName = "Silver Team"),
};

