// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterEnumLibrary.h"
#include "InventoryComponent.generated.h"

class AWeapon;
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
	UPROPERTY(EditAnywhere, Category="Weapons")
	TMap<EAmmoType, int32> Ammo;
	
	UPROPERTY(EditAnywhere, Category="Weapons")
	TSet<TSubclassOf<AWeapon>> Weapons;

	// How much ammo of each type the player can carry.
	UPROPERTY(EditAnywhere, Category="Weapons")
	TMap<EAmmoType, int32> MaxAmmo;

	
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SortWeapons(EWeaponSortingMethod SortingMethod = EWeaponSortingMethod::Ascending);

	// Returns true if weapon is picked up.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddWeapon(TSubclassOf<AWeapon> WeaponToAdd);

	// Returns true if ammo is picked up.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddAmmo(EAmmoType Type, int32 Amount);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveAmmo(EAmmoType Type, int32 Amount);
	
	// ------ INVENTORY GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory|Getters")
	TSet<TSubclassOf<AWeapon>>& GetWeapons() { return Weapons; }

	UFUNCTION(BlueprintCallable, Category="Inventory|Getters")
	int32 GetAmmo(EAmmoType Type) { return Ammo.FindRef(Type); }

	UFUNCTION(BlueprintCallable, Category="Inventory|Getters")
	int32 GetMaxAmmo(EAmmoType Type) { return MaxAmmo.FindRef(Type); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory|Getters")
	FString GetAmmoTypeName(EAmmoType Type);
	

};
