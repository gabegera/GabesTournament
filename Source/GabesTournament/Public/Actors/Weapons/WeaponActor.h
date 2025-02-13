// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class AProjectileActor;
struct FWeaponData;


UCLASS()
class GABESTOURNAMENT_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponActor();

	

protected:

	// ------ WEAPON STATS -------

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Damage = 20;

	// Fire Rate in RPM (Rounds per Minute)
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float FireRate = 60;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	bool SemiAutomatic = false;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AProjectileActor> Projectile;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// ------ GETTERS ------

	float GetDamage() { return Damage; }

	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
