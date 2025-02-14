// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class UArrowComponent;
class AProjectileActor;

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

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class GABESTOURNAMENT_API IWeaponInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fire();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SecondaryFire();
};

UCLASS()
class GABESTOURNAMENT_API AWeaponActor : public AActor, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	UArrowComponent* MuzzleArrowComponent;
	
	// ------ WEAPON STATS -------

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	EWeaponSlot WeaponSlot = EWeaponSlot::Slot1;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Damage = 20;

	// Fire Rate in RPM (Rounds per Minute)
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float FireRate = 60;

	FTimerHandle FireRateTimer;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	bool Automatic = false;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AProjectileActor> Projectile;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// ------ SHOOTING ------
	
	UFUNCTION(BlueprintCallable, Category="Shooting")
	void Fire_Implementation() override;

	UFUNCTION(Server, Reliable)
	void Server_Fire(FVector Start, FVector End, APlayerCharacter* ShotInstigator);

	UFUNCTION(BlueprintCallable, Category="Shooting")
	void SecondaryFire_Implementation() override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon Getters")
	EWeaponSlot GetWeaponSlot() { return WeaponSlot; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon Getters")
	float GetDamage() { return Damage; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon Getters")
	float GetFireRateInRPM() { return FireRate; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon Getters")
	float GetFireRateInSeconds() { return 1 / (FireRate / 60); }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};