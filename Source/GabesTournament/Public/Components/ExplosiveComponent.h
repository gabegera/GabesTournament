// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/DamageInterface.h"
#include "ExplosiveComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GABESTOURNAMENT_API UExplosiveComponent : public UActorComponent, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UExplosiveComponent();

protected:

	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionDamage = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionRadius = 128.0f;

	// The Strength of the Impulse Force on physics objects that gets multiplied by the Damage.
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionForceMultiplier = 100.0f;

	// How long it takes for the projectile to explode when fired. Timer Disabled if Set to 0.
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float FuseTime = 3.0f;

	UPROPERTY(BlueprintReadWrite, Category="Explosion")
	FTimerHandle FuseTimerHandle;

	// This timer is used to delay the destruction of the owning Actor.
	// This helps prevent pointers breaking and crashes.
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle DestructionTimerHandle;

	UPROPERTY()
	AActor* ExplosionCauser;

	UPROPERTY()
	AController* ExplosionInstigator;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void LightFuse();
	
	UFUNCTION(BlueprintCallable)
	void Explode();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_Explode();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_Explode();

	UFUNCTION(BlueprintCallable)
	void DisableOwner();

	UFUNCTION(BlueprintCallable)
	void DestroyOwner();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_DestroyOwner();

	// ------ SETTERS ------

	UFUNCTION(BlueprintCallable)
	void SetExplosionCauser(AActor* Causer) { ExplosionCauser = Causer; }

	UFUNCTION(BlueprintCallable)
	void SetExplosionInstigator(AController* Instigator) { ExplosionInstigator = Instigator; }

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetExplosionRadius() { return ExplosionRadius; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetExplosionDamage() { return ExplosionDamage; }
};
