// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Actors/Weapons/Weapon.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxAmmo.Add(EAmmoType::AssaultRifleBullets, 100);
	MaxAmmo.Add(EAmmoType::AssaultRifleGrenades, 100);
	MaxAmmo.Add(EAmmoType::BioRifle, 80);
	MaxAmmo.Add(EAmmoType::ShockRifle, 30);
	MaxAmmo.Add(EAmmoType::LinkGun, 100);
	MaxAmmo.Add(EAmmoType::Minigun, 120);
	MaxAmmo.Add(EAmmoType::Flak, 20);
	MaxAmmo.Add(EAmmoType::Rockets, 9);
	MaxAmmo.Add(EAmmoType::SniperRifle, 16);
	MaxAmmo.Add(EAmmoType::Lightning, 16);
	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	SortWeapons();

	for (TPair i : MaxAmmo)
	{
		AddAmmo(i.Key, i.Value);
	}
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UInventoryComponent::SortWeapons(EWeaponSortingMethod SortingMethod)
{
	if (SortingMethod == EWeaponSortingMethod::Ascending)
	{
		Weapons.Sort([](const TSubclassOf<AWeapon>& a , const TSubclassOf<AWeapon>& b){
			return a->GetDefaultObject<AWeapon>()->GetWeaponSlot() < b->GetDefaultObject<AWeapon>()->GetWeaponSlot();
		});
	}
	else
	{
		Weapons.Sort([](const TSubclassOf<AWeapon>& a , const TSubclassOf<AWeapon>& b){
			return a->GetDefaultObject<AWeapon>()->GetWeaponSlot() > b->GetDefaultObject<AWeapon>()->GetWeaponSlot();
		});
	}
}

bool UInventoryComponent::AddWeapon(TSubclassOf<AWeapon> WeaponToAdd)
{
	AWeapon* DefaultWeapon = WeaponToAdd.GetDefaultObject();
	if (DefaultWeapon == nullptr) return false;

	if (DefaultWeapon->GetPrimaryAmmoType() != EAmmoType::None)
	{
		AddAmmo(DefaultWeapon->GetPrimaryAmmoType(), DefaultWeapon->GetPrimaryPickupAmmo());
	}

	if (DefaultWeapon->GetSecondaryAmmoType() != EAmmoType::None && DefaultWeapon->GetSecondaryAmmoType() != DefaultWeapon->GetPrimaryAmmoType())
	{
		AddAmmo(DefaultWeapon->GetSecondaryAmmoType(), DefaultWeapon->GetSecondaryPickupAmmo());
	}

	Weapons.Add(WeaponToAdd);

	return true;
}

bool UInventoryComponent::AddAmmo(EAmmoType Type, int32 Amount)
{
	if (GetAmmo(Type) >= GetMaxAmmo(Type)) return false;
	
	Ammo.Add(Type, Ammo.FindRef(Type) + Amount);

	// Clamps Minimum value to 0.
	if (Ammo.FindRef(Type) < 0) Ammo.Add(Type, 0);
	
	// Clamps Maximum value to MaxAmmo.
	if (Ammo.FindRef(Type) > GetMaxAmmo(Type))
	{
		Ammo.Add(Type, GetMaxAmmo(Type));
	}

	FString DebugMessage = "Added " + FString::FromInt(GetAmmo(Type)) + " " + GetAmmoTypeName(Type) + " Ammo.";
	GEngine->AddOnScreenDebugMessage(200, 3.0f, FColor::Yellow, DebugMessage);
	return true;
}

bool UInventoryComponent::RemoveAmmo(EAmmoType Type, int32 Amount)
{
	if (GetAmmo(Type) <= 0) return false;

	Ammo.Add(Type, GetAmmo(Type) - Amount);

	// Clamps Minimum value to 0.
	if (Ammo.FindRef(Type) < 0) Ammo.Add(Type, 0);
	
	// Clamps Maximum value to MaxAmmo.
	if (Ammo.FindRef(Type) > GetMaxAmmo(Type))
	{
		Ammo.Add(Type, GetMaxAmmo(Type));
	}

	return true;
}

FString UInventoryComponent::GetAmmoTypeName(EAmmoType Type)
{
	switch (Type)
	{
		case EAmmoType::AssaultRifleBullets:
			return "Assault Rifle Bullets";
		case EAmmoType::AssaultRifleGrenades:
			return "Assault Rifle Grenades";
		case EAmmoType::BioRifle:
			return "Bio Rifle";
		case EAmmoType::ShockRifle:
			return "Shock Rifle";
		case EAmmoType::LinkGun:
			return "Link Gun";
		case EAmmoType::Minigun:
			return "Minigun";
		case EAmmoType::Flak:
			return "Flak";
		case EAmmoType::Rockets:
			return "Rockets";
		case EAmmoType::SniperRifle:
			return "Sniper Rifle";
		case EAmmoType::Lightning:
			return "Lightning";
		default:
			UE_LOG(LogTemp, Warning, TEXT("Missing Name for AmmoType: %hhd"), Type);
			return "ERROR: Missing Name for AmmoType";
	}
}



