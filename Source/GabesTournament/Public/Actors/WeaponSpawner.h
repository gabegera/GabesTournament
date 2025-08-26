// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Weapon.h"
#include "WeaponSpawner.generated.h"

UCLASS()
class GABESTOURNAMENT_API AWeaponSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponSpawner();

protected:

	UPROPERTY(EditAnywhere, Category="Spawner|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	// Where the Weapon will be spawned.
	UPROPERTY(EditAnywhere, Category = "Spawner|Components")
	TObjectPtr<class UArrowComponent> SpawnerArrowComponent;

	// Mesh to show what weapon will be spawning here in the editor.
	UPROPERTY(EditAnywhere, Category="Spawner|Components")
	TObjectPtr<USkeletalMeshComponent> WeaponEditorMesh;

	UPROPERTY(EditAnywhere, Category="Spawner")
	TSubclassOf<AWeapon> WeaponToSpawn;

	UPROPERTY()
	TObjectPtr<AWeapon> SpawnedWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Spawner")
	float SpawnDelay = 3.0f;

	UPROPERTY()
	FTimerHandle SpawnTimer;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SpawnWeapon();
};
