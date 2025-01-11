// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "DataTables/WeaponData.h"
#include "WeaponActor.generated.h"



UCLASS()
class GABESTOURNAMENT_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponActor();

protected:

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	UArrowComponent* MuzzleArrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Data", meta=(RowType="WeaponData"))
	FDataTableRowHandle WeaponDataTableRow;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle FireRateTimer;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Leave ProjectileActor NULL to shoot Hitscan.
	UFUNCTION(BlueprintCallable)
	virtual void Shoot(FVector StartingPosition, FVector Target, float SpreadInDegrees, float Damage, float Range, EAmmoType AmmoType, AProjectileActor* ProjectileActor = nullptr);

	UFUNCTION(BlueprintCallable)
	void ShootHitscan(FVector StartingPosition, FVector Target, float SpreadInDegrees, float Damage);

	UFUNCTION(BlueprintCallable)
	void ShootProjectile(AProjectileActor* ProjectileActor, FVector StartingPosition, FVector Target, float SpreadInDegrees, float Damage);

	UFUNCTION(BlueprintCallable)
	virtual void UseSecondaryAction();

	UFUNCTION(BlueprintCallable)
	bool CanShoot();

	// ------ GETTERS ------
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetWeaponData() { return *WeaponDataTableRow.GetRow<FWeaponData>(""); }
};