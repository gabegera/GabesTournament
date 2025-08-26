// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DamageInterface.h"
#include "ShooterEnumLibrary.h"
#include "DataTables/WeaponData.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum EPrimaryOrSecondary : uint8
{
	Primary = 0 UMETA(DisplayName = "Primary"),
	Secondary = 1 UMETA(DisplayName = "Secondary"),
	Null = 2 UMETA(DisplayName = "NULL"),
};

UCLASS()
class GABESTOURNAMENT_API AWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	class APlayerCharacter* CharacterOwner = nullptr;

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Components")
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Components")
	TObjectPtr<class UArrowComponent> MuzzleArrowComponent;

	// ------ PICKUP ------

	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<class USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, Category="Weapon|Pickup")
	bool isPickup;

	UPROPERTY(EditAnywhere, Category="Weapon|Pickup", meta=(EditCondition="isPickup"))
	int32 PrimaryAmmoPickupAmount = 0;

	UPROPERTY(EditAnywhere, Category="Weapon|Pickup", meta=(EditCondition="isPickup && SecondaryAmmoType != PrimaryAmmoType && SecondaryAmmoType != EAmmoType::None"))
	int32 SecondaryAmmoPickupAmount = 0;
	
	// ------ WEAPON STATS -------

	UPROPERTY(EditAnywhere)
	FString DisplayName = "WeaponName_NULL";

	UPROPERTY(EditAnywhere)
	EWeaponSlot WeaponSlot = EWeaponSlot::Slot1;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle FireRateTimer;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle BurstTimer;

	UPROPERTY()
	int32 CurrentBurstCount = 0;

	UPROPERTY()
	float PrimaryCurrentCharge = 0.0f;

	UPROPERTY()
	float SecondaryCurrentCharge = 0.0f;

	UPROPERTY()
	bool isPrimaryTriggerPulled = false;

	UPROPERTY()
	bool isSecondaryTriggerPulled = false;

	// ------ CHARGING ------


	// ------ OVERHEATING / COOLDOWN ------

	UPROPERTY()
	FTimerHandle CooldownDelayTimer;

	UPROPERTY()
	float CurrentOverheatValue = 0.0f;

	UPROPERTY()
	bool isOverheated = false;

	UPROPERTY()
	bool isCoolingDown = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	// ------ SHOOTING ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCanFire(const EPrimaryOrSecondary PrimaryOrSecondaryFire);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector VectorWithSpreadFromPoints(FVector StartLocation, FVector EndLocation, float Spread);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ConvertFireRateToSeconds(float FireRateRPM) { return 1 / (FireRateRPM / 60); }

	UFUNCTION()
	void PullPrimaryTrigger();
	
	UFUNCTION()
	void ReleasePrimaryTrigger();

	UFUNCTION()
	void PullSecondaryTrigger();

	UFUNCTION()
	void ReleaseSecondaryTrigger();

	UFUNCTION(BlueprintImplementableEvent)
	void Fire(EPrimaryOrSecondary Primary = Primary);

	UFUNCTION(BlueprintImplementableEvent)
	void SecondaryFire(EPrimaryOrSecondary Secondary = Secondary);

	// Charges the weapon before shooting. Returns true when fully charged.
	UFUNCTION(BlueprintCallable, Category="Weapon|Shooting")
	bool ChargeWeapon(EPrimaryOrSecondary PrimaryOrSecondary, float TimeToCharge = 1.0f);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Weapon|Shooting", DisplayName="Shoot Projectile")
	void Server_ShootProjectile(EPrimaryOrSecondary PrimaryOrSecondary = Primary, EAmmoType AmmoType = EAmmoType::None, int32 AmmoCost = 1, TSubclassOf<AProjectile> Projectile = nullptr, int32 ProjectilesToSpawn = 1, float ProjectileSpeed = 5000.0f, float Damage = 10.0f, float KnockbackForceMultiplier = 100.0f,
						float SpreadInDegrees = 0.0f, bool isAutomatic = true, float FireRateRPM = 300.0f, bool DoesOverheat = false);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Weapon|Shooting", DisplayName="Shoot Projectile Burst")
	void Server_ShootProjectileBurst(EPrimaryOrSecondary PrimaryOrSecondary = Primary, EAmmoType AmmoType = EAmmoType::None, int32 AmmoCost = 1, TSubclassOf<AProjectile> Projectile = nullptr, int32 ProjectilesToSpawn = 1, int32 BurstCount = 3, float BurstDelay = 0.1f, float ProjectileSpeed = 5000.0f,
							  float Damage = 10.0f, float KnockbackForceMultiplier = 100.0f, float SpreadInDegrees = 0.0f, bool isAutomatic = true, float FireRateRPM = 300.0f, bool DoesOverheat = false);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Weapon|Shooting", DisplayName="Shoot Hitscan")
	void Server_ShootHitscan(EPrimaryOrSecondary PrimaryOrSecondary = Primary, EAmmoType AmmoType = EAmmoType::None, int32 AmmoCost = 1, int32 ProjectilesToSpawn = 1, float Damage = 10.0f, float KnockbackForceMultiplier = 100.0f, float SpreadInDegrees = 0.0f, bool isAutomatic = true,
							 float FireRateRPM = 300.0f, bool DoesOverheat = false);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Weapon|Shooting", DisplayName="Shoot Hitscan Burst")
	void Server_ShootHitscanBurst(EPrimaryOrSecondary PrimaryOrSecondary = Primary, EAmmoType AmmoType = EAmmoType::None, int32 AmmoCost = 1, int32 ProjectilesToSpawn = 1, int32 BurstCount = 3, float BurstDelay = 0.1f, float Damage = 10.0f, float KnockbackForceMultiplier = 100.0f,
								  float SpreadInDegrees = 0.0f, bool isAutomatic = true, float FireRateRPM = 300.0f, bool DoesOverheat = false);

	// UFUNCTION(Server, Reliable, BlueprintCallable)
	// void Server_ShootHitscan(float Damage, float KnockbackForceMultiplier, FVector Start, FVector End, ACharacter* Causer);

	// UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	// void Multicast_ShootHitscan(float Damage, FVector Start, FVector End, ACharacter* Causer);

	// UFUNCTION(Server, Reliable, BlueprintCallable)
	// void Server_ShootProjectile(TSubclassOf<AProjectile> ProjectileActor, float Damage, float KnockbackForceMultiplier, FVector Start, FVector Target, ACharacter* Causer);
	//
	// UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	// void Multicast_ShootProjectile(TSubclassOf<AProjectile> ProjectileActor, float Damage, FVector Start, FVector Target, ACharacter* Causer);

	// ------ OVERHEATING / COOLDOWN ------

	UFUNCTION(BlueprintCallable)
	void StartCooldown();
	
	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	APlayerCharacter* GetCharacterOwner();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	USkeletalMeshComponent* GetMeshComponent() { return MeshComponent;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	USkeletalMesh* GetMesh() { return MeshComponent->GetSkeletalMeshAsset(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	EWeaponSlot GetWeaponSlot() { return WeaponSlot; }

	// Returns how much primary ammo will be added when a weapon is picked up.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	int32 GetPrimaryPickupAmmo() { return PrimaryAmmoPickupAmount; }

	// Returns how much secondary ammo will be added when a weapon is picked up.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	int32 GetSecondaryPickupAmmo() { return PrimaryAmmoPickupAmount; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	float GetCurrentOverheatValue() const { return CurrentOverheatValue; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	bool GetIsOverheated() const { return isOverheated; }

	// UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	// float GetCurrentCharge(EPrimaryOrSecondary PrimaryOrSecondary);

	// UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	// float GetMaxCharge(EPrimaryOrSecondary PrimaryOrSecondary);

	// ------ SETTERS ------

	bool SetIsPickup(bool Input) { return isPickup = Input; }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};