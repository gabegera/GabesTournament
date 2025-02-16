// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthActorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GABESTOURNAMENT_API UHealthActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthActorComponent();

protected:

	UPROPERTY(EditAnywhere, Replicated, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100.0f;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
	float AddHealth(float HealthToAdd) { return Health += HealthToAdd; }

	UFUNCTION(BlueprintCallable)
	float TakeDamage(float DamageAmount);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_TakeDamage(float DamageAmount);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_TakeDamage(float DamageAmount);

		
};
