// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/Pickup.h"
#include "Interfaces/DamageInterface.h"
#include "Components/HealthComponent.h"
#include "Components/InventoryComponent.h"
#include "ShooterDelegates.h"
#include "ShooterPlayerState.h"
#include "GameFramework/Character.h"
#include "Actors/Weapons/Weapon.h"
#include "ShooterCharacter.generated.h"

class AShooterPlayerState;
class AWeapon;

UCLASS()
class GABESTOURNAMENT_API AShooterCharacter : public ACharacter, public IPickupInterface, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:

	// ------ COMPONENTS ------

	UPROPERTY(EditDefaultsOnly, Category="Health")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category="Weapons", meta=(AllowedClasses="AWeaponActor"))
	TObjectPtr<UChildActorComponent> WeaponChildActorComponent;

	UPROPERTY(EditDefaultsOnly, Category="Meshes")
	TObjectPtr<USkeletalMeshComponent> ThirdPersonWeaponMeshComponent;

	// ------ INVENTORY ------

	UPROPERTY()
	TObjectPtr<AWeapon> EquippedWeapon;

	// ------ DELEGATES ------
	
	UPROPERTY(BlueprintReadOnly)
	FCharacterHasDied CharacterHasDiedDelegate;

	// ------ DEATH ------

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeToDespawnAfterDeath = 30.0f;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual bool DealDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool AddRadialImpulse_Implementation(FVector ImpulseLocation, float Radius, float Force, bool Falloff = true) override;

	virtual bool AddImpulseAtLocation_Implementation(FVector Impulse, FVector Location, FName BoneName = NAME_None) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Weapons")
	bool OwnerOnlySeeWeapon = true;

	UFUNCTION(BlueprintCallable)
	virtual void SetMeshColor(FColor Color);

	// Function called at BeginPlay. If true other players Weapon Children Components will be hidden from each other.
	UFUNCTION(Server, Reliable)
	void Server_OwnerOnlySeeWeapon(bool isTrue);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetIsOtherWeaponsHidden(bool isHidden);

	UFUNCTION(Client, Reliable)
	void Client_SetIsOwnerWeaponHidden(bool isHidden);

	UFUNCTION(BlueprintCallable)
	virtual void Die(FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void ActivateRagdoll();

	UFUNCTION(BlueprintCallable)
	void DeactivateRagdoll();

	UFUNCTION(BlueprintCallable)
	void StartDespawnTimer(float Seconds);

	UFUNCTION(BlueprintCallable)
	void Despawn();

	UFUNCTION(BlueprintCallable)
	void AddDeathToKillfeed(ACharacter* DeadCharacter, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	// ------ PICKUP FUNCTIONS ------

	// UFUNCTION(BlueprintCallable)
	// bool PickupWeapon_Implementation(TSubclassOf<AWeapon> Weapon) override;

	UFUNCTION(BlueprintCallable)
	bool PickupAmmo_Implementation(EAmmoType AmmoType, int32 AmmoAmount) override;

	// UFUNCTION(BlueprintCallable)
	// bool PickupHealth_Implementation(float Amount) override;
	//
	// UFUNCTION(BlueprintCallable)
	// bool PickupShield_Implementation(float Amount) override;
	//
	// UFUNCTION(BlueprintCallable)
	// bool PickupDamageBoost_Implementation() override;
	//
	// UFUNCTION(BlueprintCallable)
	// bool PickupSpeedBoost_Implementation() override;

	// ------ SETTERS ------

	UFUNCTION(BlueprintCallable)
	AWeapon* SetEquippedWeapon(AWeapon* Weapon) { return EquippedWeapon = Weapon; }

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character|Getters")
	float GetHealth() { return HealthComponent->GetHealth(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character|Getters")
	UHealthComponent* GetHealthComponent() { return HealthComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character|Getters")
	UInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character|Getters")
	UChildActorComponent* GetWeaponChildActorComponent() { return WeaponChildActorComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character|Getters")
	USkeletalMeshComponent* GetThirdPersonWeaponComponent() { return ThirdPersonWeaponMeshComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character|Getters")
	AWeapon* GetEquippedWeapon();

};
