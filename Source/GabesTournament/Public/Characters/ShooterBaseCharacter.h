// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthActorComponent.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"

class AWeaponActor;

UCLASS()
class GABESTOURNAMENT_API AShooterBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterBaseCharacter();

protected:

	// ------ COMPONENTS ------

	UPROPERTY(EditDefaultsOnly, Category="Health")
	class UHealthActorComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category="Weapons", meta=(AllowedClasses="AWeaponActor"))
	UChildActorComponent* WeaponChildActorComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Weapons")
	bool OwnerOnlySeeWeapon = true;

	// Function called at BeginPlay. If true other players Weapon Children Components will be hidden from each other.
	UFUNCTION(Server, Reliable)
	void Server_OwnerOnlySeeWeapon(bool isTrue);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_IsOtherWeaponsHidden(bool isHidden);

	UFUNCTION(Client, Reliable)
	void Client_IsOwnerWeaponHidden(bool isHidden);

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character Getters")
	float GetHealth() { return HealthComponent->GetHealth(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character Getters")
	UHealthActorComponent* GetHealthComponent() { return HealthComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character Getters")
	UInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Shooter Character Getters")
	UChildActorComponent* GetWeaponChildActorComponent() { return WeaponChildActorComponent; }

};
