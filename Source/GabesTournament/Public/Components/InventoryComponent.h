// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTables/WeaponData.h"
#include "InventoryComponent.generated.h"

class AWeaponActor;
enum class EAmmoType : uint8;

UENUM(BlueprintType)
enum class EWeaponSortingMethod : uint8
{
	Ascending = 0 UMETA(DisplayName = "Ascending"),
	Descending = 1 UMETA(DisplayName = "Descending"),
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GABESTOURNAMENT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:

	// Ammo being carried.
	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TMap<EAmmoType, int32> Ammo;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TSet<TSubclassOf<AWeaponActor>> Weapons;

	// How much ammo of each type the player can carry.
	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TMap<EAmmoType, int32> MaxAmmo;

	
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category="Inventory Getters")
	void SortWeapons(EWeaponSortingMethod SortingMethod = EWeaponSortingMethod::Ascending);	

	// ------ INVENTORY GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory Getters")
	TSet<TSubclassOf<AWeaponActor>>& GetWeapons() { return Weapons; }

	UFUNCTION(BlueprintCallable, Category="Inventory Getters")
	int32 GetAmmo(EAmmoType AmmoType) { return Ammo[AmmoType]; }

	UFUNCTION(BlueprintCallable, Category="Inventory Getters")
	int32 GetMaxAmmo(EAmmoType AmmoType) { return MaxAmmo[AmmoType]; }

	
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
