// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Characters/ShooterCharacter.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (RefreshWeaponsDataTable == true)
	{
		RefreshWeaponsDataTable = false;
	}
	
	if (WeaponsDataTable == nullptr) return;

	TArray<FWeaponData*> WeaponDataTableRows;
	WeaponsDataTable->GetAllRows<FWeaponData>("", WeaponDataTableRows);
	RefreshOwnedWeapons();

	AmmoMap.Empty();
	OwnedWeapons.Empty();
	for (int32 i = 0; i < StartingWeapons.Num(); i++)
	{
		if (StartingWeapons.FindRef(WeaponDataTableRows[i]->DisplayName) == true)
		{
			OwnedWeapons.Add(*WeaponDataTableRows[i]);

			AddAmmo(WeaponDataTableRows[i]->PrimaryAmmoType, WeaponDataTableRows[i]->PrimaryStartingAmmo);
			if (WeaponDataTableRows[i]->PrimaryAmmoType != WeaponDataTableRows[i]->SecondaryAmmoType)
			{
				AddAmmo(WeaponDataTableRows[i]->SecondaryAmmoType, WeaponDataTableRows[i]->SecondaryStartingAmmo);
			}
		}
	}
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::RefreshOwnedWeapons()
{
	TArray<FWeaponData*> WeaponDataTableRows;
	WeaponsDataTable->GetAllRows<FWeaponData>("", WeaponDataTableRows);
	TMap<FName, bool> NewMap;
	
	for (int32 i = 0; i < WeaponDataTableRows.Num(); i++)
	{
		const FName Key = WeaponDataTableRows[i]->DisplayName;
		const bool Value = StartingWeapons.FindRef(Key);
		NewMap.Add(Key, Value);
	}
	
	StartingWeapons.Empty();
	StartingWeapons.Append(NewMap);
}

void UInventoryComponent::AddWeapon(const FWeaponData& NewWeapon)
{
	
}

void UInventoryComponent::AddAmmo(EAmmoType AmmoType, int32 AmmoAmount)
{
	
}





