// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Components/InventoryComponent.h"
#include "AmmoPickup.generated.h"

UCLASS()
class GABESTOURNAMENT_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAmmoPickup();

protected:

	UPROPERTY(EditDefaultsOnly, Category="Ammo Pickup")
	EAmmoType AmmoType = EAmmoType::None;

	UPROPERTY(EditDefaultsOnly, Category="Ammo Pickup")
	int32 AmmoAmount = 20;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
