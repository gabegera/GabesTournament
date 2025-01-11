// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTables/WeaponData.h"
#include "Components/HealthComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectileActor.generated.h"

UCLASS()
class GABESTOURNAMENT_API AProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollision;
	
	UFUNCTION(BlueprintCallable)
	void DealDamage(float Damage, AActor* Collider = nullptr, AActor* PlayerCharacter = nullptr, AController* PlayerController = nullptr);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
