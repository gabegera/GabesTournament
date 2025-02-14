// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "DataTables/WeaponData.h"


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
}

void UInventoryComponent::SortWeapons(EWeaponSortingMethod SortingMethod)
{
	if (SortingMethod == EWeaponSortingMethod::Ascending)
	{
		Weapons.Sort([](const TSubclassOf<AWeaponActor>& a , const TSubclassOf<AWeaponActor>& b){
			return a->GetDefaultObject<AWeaponActor>()->GetWeaponSlot() < b->GetDefaultObject<AWeaponActor>()->GetWeaponSlot();
		});
	}
	else
	{
		Weapons.Sort([](const TSubclassOf<AWeaponActor>& a , const TSubclassOf<AWeaponActor>& b){
			return a->GetDefaultObject<AWeaponActor>()->GetWeaponSlot() > b->GetDefaultObject<AWeaponActor>()->GetWeaponSlot();
		});
	}
	
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

