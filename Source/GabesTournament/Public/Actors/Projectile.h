// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ExplosiveComponent.h"
#include "Interfaces/DamageInterface.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class GABESTOURNAMENT_API AProjectile : public AActor, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	class UExplosiveComponent* ExplosiveComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Impact Settings")
	bool DoesDealImpactDamage = true;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Impact Settings")
	float ImpactDamage;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Impact Settings")
	float KnockbackForceMultiplier;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Explosion Settings")
	bool DoesExplode = false;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Explosion Settings")
	bool DoesExplodeOnCollision = true;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile|Spawned Projectile Settings")
	bool DoesSpawnProjectilesOnCollision = false;

	// Which Projectile Actor will be spawned on collision.
	UPROPERTY(EditDefaultsOnly, Category="Projectile|Spawned Projectile Settings")
	TSubclassOf<AProjectile> ProjectilesToSpawn;

	// How many projectiles will be spawned on collision.
	UPROPERTY(EditDefaultsOnly, Category="Projectile|Spawned Projectile Settings")
	int32 ProjectilesToSpawnCount;

	// How many projectiles will be spawned on collision.
	UPROPERTY(EditDefaultsOnly, Category="Projectile|Spawned Projectile Settings")
	float ProjectilesToSpawnDamage;

	// The Actor that Spawned the Projectile
	UPROPERTY()
	AActor* Spawner;

	// ------ COLLISION CHECK ------

	UPROPERTY()
	FVector CurrentLocation = FVector::ZeroVector;

	UPROPERTY()
	FVector PreviousLocation = FVector::ZeroVector;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CheckCollision();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_DealImpactDamage(AActor* Target, float Damage, AController* DamageInstigator = nullptr, AActor* DamageCauser = nullptr);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnProjectiles(TSubclassOf<AProjectile> Projectile, int32 NumberToSpawn, float Damage, FVector UpSpawnDirection, AController* DamageInstigator = nullptr, AActor* DamageCauser = nullptr);

	// ------ SETTERS ------

	// Set the Actor that Spawned the Projectile.
	UFUNCTION(BlueprintCallable)
	void SetSpawner(AActor* Input) { Spawner = Input; }

	UFUNCTION(BlueprintCallable)
	float SetImpactDamage(float Damage) { return ImpactDamage = Damage; }

	UFUNCTION(BlueprintCallable)
	float SetKnockbackForceMultiplier(float Force) { return KnockbackForceMultiplier = Force; }

	
	// ------ GETTERS ------

	// Get the Actor that Spawned the Projectile.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetSpawner() { return Spawner; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UProjectileMovementComponent* GetProjectileMovement() { return ProjectileMovement; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetImpactDamage() { return ImpactDamage; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetExplosionRadius() { return ExplosiveComponent->GetExplosionRadius(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetExplosionDamage() { return ExplosiveComponent->GetExplosionDamage(); }
};
