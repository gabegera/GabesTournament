// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

enum class EAmmoType : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

class GABESTOURNAMENT_API IPickupInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupAmmo(EAmmoType AmmoType, int32 AmmoAmount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupHealth(float Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupShield(float Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupDamageBoost();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupSpeedBoost();
};

UCLASS()
class GABESTOURNAMENT_API APickup : public AActor, public IPickupInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup();

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly)
	float SpawnDelay = 10.0f;

	UPROPERTY()
	FTimerHandle SpawnTimer;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void HidePickup() { StaticMeshComponent->SetVisibility(false); }

	UFUNCTION(BlueprintCallable)
	void ShowPickup() { StaticMeshComponent->SetVisibility(true); }
};
