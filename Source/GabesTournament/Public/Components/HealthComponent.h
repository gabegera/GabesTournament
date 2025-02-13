// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HealthComponent.generated.h"

UCLASS()
class GABESTOURNAMENT_API UHealthComponent : public UObject
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;
	
	UPROPERTY()
	float Health = MaxHealth;

public:

	UFUNCTION(BlueprintCallable)

	void TakeDamage(const float DamageAmount);

	void AddHealth(const float HealthAmount) { Health += HealthAmount; }

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() { return Health; }
	
	
};
