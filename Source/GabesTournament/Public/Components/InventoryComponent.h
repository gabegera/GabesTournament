// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTables/WeaponData.h"
#include "InventoryComponent.generated.h"

class AWeaponActor;
enum class EAmmoType : uint8;

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

	// How much ammo of each type the player can carry.
	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TMap<EAmmoType, int32> MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TSet<TSubclassOf<AWeaponActor>> Weapons;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
