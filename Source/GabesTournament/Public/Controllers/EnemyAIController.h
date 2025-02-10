// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Characters/EnemyCharacter.h"
#include "Components/ArrowComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnemyAIController.generated.h"



UCLASS()
class GABESTOURNAMENT_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:

	ABaseEnemyAIController();

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY()
	AEnemyCharacter* EnemyCharacter;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// UAIPerceptionComponent* AIPerceptionComponent = nullptr;

	UPROPERTY()
	FTimerHandle FireRateTimerHandle;

	UPROPERTY()
	bool CanShoot = true;

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AEnemyCharacter* GetEnemyCharacter();

	// UFUNCTION(BlueprintCallable)
	// void Shoot(FVector TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetShotSpreadInDegrees();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCanShoot() { return CanShoot; }

	UFUNCTION(BlueprintCallable)
	void CanShootEnable() { CanShoot = true; }

	UFUNCTION(BlueprintCallable)
	void CanShootDisable() { CanShoot = false;}

	// ------ WEAPON DATA GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetEquippedWeapon();

	// UFUNCTION(BlueprintCallable, BlueprintPure)
	// EAmmoType GetAmmoType() { return GetEquippedWeapon().AmmoType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSoftObjectPtr<AProjectileActor> GetProjectileActor() { return GetEquippedWeapon().ProjectileActor; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EProjectileType GetProjectileType() { return GetEquippedWeapon().ProjectileType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetMuzzleLocation() { return GetEnemyCharacter()->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>()->GetComponentLocation(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetFireRate() { return GetEquippedWeapon().FireRate; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDamage() { return GetEquippedWeapon().Damage; }

	virtual void Tick(float DeltaTime) override;
};
