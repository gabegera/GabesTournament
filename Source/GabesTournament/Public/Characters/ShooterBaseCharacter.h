// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"

class AWeaponActor;

UCLASS()
class GABESTOURNAMENT_API AShooterBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterBaseCharacter();

protected:

	// ------ COMPONENTS ------

	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	UChildActorComponent* WeaponChildActorComponent;

	UPROPERTY(EditDefaultsOnly, Category="Health")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	class UInventoryComponent* InventoryComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
