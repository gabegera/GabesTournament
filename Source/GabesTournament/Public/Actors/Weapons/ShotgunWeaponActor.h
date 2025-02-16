// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "ShotgunWeaponActor.generated.h"

UCLASS()
class GABESTOURNAMENT_API AShotgunWeaponActor : public AWeaponActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShotgunWeaponActor();

protected:

	// How many shotgun projectiles will be fired.
	UPROPERTY(EditDefaultsOnly)
	int32 ShotCount = 8;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Fire_Implementation() override;

	void SecondaryFire_Implementation() override;
};
