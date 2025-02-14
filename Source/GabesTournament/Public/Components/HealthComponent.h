// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GABESTOURNAMENT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void PostInitProperties() override;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;
	
	UPROPERTY()
	float Health = MaxHealth;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(const float DamageAmount);

	void AddHealth(const float HealthAmount) { Health += HealthAmount; }

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() { return Health; }
};
