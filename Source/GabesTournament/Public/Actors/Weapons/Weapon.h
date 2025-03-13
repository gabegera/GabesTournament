// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DamageInterface.h"
#include "ShooterEnumLibrary.h"
#include "DataTables/WeaponData.h"
#include "Weapon.generated.h"

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReleasePrimaryTrigger();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReleaseSecondaryTrigger();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupWeapon(TSubclassOf<AWeapon> Weapon);
};

UENUM(BlueprintType)
enum EPrimaryOrSecondary : uint8
{
	Primary = 0 UMETA(DisplayName = "Primary"),
	Secondary = 1 UMETA(DisplayName = "Secondary")
};

UCLASS()
class GABESTOURNAMENT_API AWeapon : public AActor, public IWeaponInterface, public IDamageInterface
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(RowType = "WeaponData"))
	FDataTableRowHandle WeaponDataTable;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle PrimaryFireRateTimer;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle SecondaryFireRateTimer;

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

	// Used to ensure that a semi auto weapon only fires once per trigger pull.
	UPROPERTY()
	bool hasPrimarySemiAutoFired = false;

	UPROPERTY()
	bool isSecondaryTriggerPulled = false;

	// Used to ensure that a semi auto weapon only fires once per trigger pull.
	UPROPERTY()
	bool hasSecondarySemiAutoFired = false;

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
	FVector GetShotTargetWithSpread(float Spread);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ConvertFireRateToSeconds(float FireRateRPM) { return 1 / (FireRateRPM / 60); }
	
	UFUNCTION(BlueprintCallable, Category="Weapon|Shooting")
	void Fire_Implementation() override;

	UFUNCTION(BlueprintCallable, Category="Weapon|Shooting")
	void ReleasePrimaryTrigger_Implementation() override;

	UFUNCTION(BlueprintCallable, Category="Weapon|Shooting")
	void SecondaryFire_Implementation() override;

	UFUNCTION(BlueprintCallable, Category="Weapon|Shooting")
	void ReleaseSecondaryTrigger_Implementation() override;

	UFUNCTION(BlueprintCallable, Category="Weapon|Shooting")
	void ChargeWeapon(EPrimaryOrSecondary PrimaryOrSecondary);

	UFUNCTION(BlueprintCallable, Category="Weapon|Shooting")
	void ShootWeapon(TSubclassOf<AProjectile> Projectile, EAmmoType AmmoType, int32 AmmoCost, float Damage, float KnockbackForceMultiplier, int32 BurstCount,
		int32 ShotgunPelletCount, float SpreadInDegrees, FTimerHandle& FireRateTimerHandle, float FireRateInSeconds, bool DoesOverheat);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ShootHitscan(float Damage, float KnockbackForceMultiplier, FVector Start, FVector End, ACharacter* Causer);

	// UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	// void Multicast_ShootHitscan(float Damage, FVector Start, FVector End, ACharacter* Causer);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ShootProjectile(TSubclassOf<AProjectile> ProjectileActor, float Damage, float KnockbackForceMultiplier, FVector Start, FVector Target, ACharacter* Causer);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_ShootProjectile(TSubclassOf<AProjectile> ProjectileActor, float Damage, FVector Start, FVector Target, ACharacter* Causer);

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
	FWeaponData GetWeaponData();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	EWeaponSlot GetWeaponSlot() { return GetWeaponData().WeaponSlot; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	FString GetDisplayName() { return GetWeaponData().DisplayName; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	float GetFireRateInRPM() { return GetWeaponData().PrimaryFireRate; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	float GetFireRateInSeconds(EPrimaryOrSecondary PrimaryOrSecondary);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	EAmmoType GetPrimaryAmmoType() { return GetWeaponData().PrimaryAmmoType; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	EAmmoType GetSecondaryAmmoType() { return GetWeaponData().SecondaryAmmoType; }

	// Returns how much primary ammo will be added when a weapon is picked up.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	int32 GetPrimaryPickupAmmo() { return PrimaryAmmoPickupAmount; }

	// Returns how much secondary ammo will be added when a weapon is picked up.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	int32 GetSecondaryPickupAmmo() { return PrimaryAmmoPickupAmount; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	float GetCurrentOverheatValue() const { return CurrentOverheatValue; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	float GetOverheatThreshold() { return GetWeaponData().OverheatThreshold; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	bool GetIsOverheated() const { return isOverheated; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	float GetCurrentCharge(EPrimaryOrSecondary PrimaryOrSecondary);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon|Getters")
	float GetMaxCharge(EPrimaryOrSecondary PrimaryOrSecondary);

	// ------ SETTERS ------

	bool SetIsPickup(bool Input) { return isPickup = Input; }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};