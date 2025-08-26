// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Characters/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/Weapons/Weapon.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "ShooterPlayerController.generated.h"

enum class EWeaponSlot : uint8;

UCLASS()
class GABESTOURNAMENT_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

	// Do not Call, used for caching. 
	UPROPERTY() APlayerCharacter* PlayerCharacter = nullptr; // Use GetPlayerCharacter() instead.
	
public:
	AShooterPlayerController();

protected:
	
	// ------ ENHANCED INPUT ------

		// ------ MOVEMENT INPUT ------
	
	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_Move;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_Look;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_Jump;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_Dash;
	
		// ------ WEAPONS INPUT ------

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_Fire;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_ReleaseFire;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_SecondaryFire;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_ReleaseSecondaryFire;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_SwapWeapons;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_EquipSlot1;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_EquipSlot2;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_EquipSlot3;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_EquipSlot4;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_EquipSlot5;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_EquipSlot6;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_EquipSlot7;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_EquipSlot8;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Input_EquipSlot9;

	// ------ INPUT FUNCTIONS ------

	UFUNCTION(BlueprintCallable, Category="Input")
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	// ------ INPUT VARIABLES ------

	UPROPERTY(EditAnywhere, Category="Input")
	float MouseLookSensitivity = 0.2f;

	// ------ CHARACTER FUNCTIONS -------

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// ------ MOVEMENT FUNCTIONS ------

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Move(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Look(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Jump();

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Dash(const FInputActionInstance& Instance);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Movement")
	void Server_Dash(FVector DashForce);

	// ------ MOVEMENT VARIABLES ------

	UPROPERTY(EditAnywhere, Category="Movement")
	FVector DashLaunchForce = FVector(800.0f, 800.0f, 600.0f);

	UPROPERTY(EditAnywhere, Category="Movement", meta=(Units="Seconds"))
	float DashCooldown = 0.5f;
	
	FTimerHandle DashCooldownTimer;

	// ------ WEAPON FUNCTIONS ------
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ReleaseFire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SecondaryFire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ReleaseSecondaryFire();

	// Scrolls through the weapon inventory to find the next one to equip.
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SwapWeapons(const FInputActionInstance& Instance);

	// Equips a weapon from a specific slot.
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipWeaponFromSlot(EWeaponSlot WeaponSlot);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipSlot1() { EquipWeaponFromSlot(EWeaponSlot::Slot1); }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipSlot2() { EquipWeaponFromSlot(EWeaponSlot::Slot2); }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipSlot3() { EquipWeaponFromSlot(EWeaponSlot::Slot3); }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipSlot4() { EquipWeaponFromSlot(EWeaponSlot::Slot4); }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipSlot5() { EquipWeaponFromSlot(EWeaponSlot::Slot5); }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipSlot6() { EquipWeaponFromSlot(EWeaponSlot::Slot6); }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipSlot7() { EquipWeaponFromSlot(EWeaponSlot::Slot7); }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipSlot8() { EquipWeaponFromSlot(EWeaponSlot::Slot8); }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipSlot9() { EquipWeaponFromSlot(EWeaponSlot::Slot9); }

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Weapon")
	void Server_EquipWeapon(TSubclassOf<AWeapon> Weapon);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="Weapon")
	void Multicast_EquipWeapon(APlayerCharacter* Target, TSubclassOf<AWeapon> Weapon);
	

public:

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Player Controller Getters")
	APlayerCharacter* GetPlayerCharacter() { return PlayerCharacter ? PlayerCharacter : PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Player Controller Getters")
	AWeapon* GetEquippedWeapon() { return Cast<AWeapon>(GetPlayerCharacter()->GetWeaponChildActorComponent()->GetChildActor()); }
	
};
