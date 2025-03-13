// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DamageInterface.h"
#include "ShooterActor.generated.h"

UCLASS()
class GABESTOURNAMENT_API AShooterActor : public AActor, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShooterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="LevelProp|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category="LevelProp|Components")
	TObjectPtr<class UHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, Category="LevelProp|Components")
	TObjectPtr<class UExplosiveComponent> ExplosiveComponent;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool DealDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool AddRadialImpulse_Implementation(FVector ImpulseLocation, float Radius, float Force, bool Falloff = true) override;

	virtual bool AddImpulseAtLocation_Implementation(FVector Impulse, FVector Location, FName BoneName = NAME_None) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
