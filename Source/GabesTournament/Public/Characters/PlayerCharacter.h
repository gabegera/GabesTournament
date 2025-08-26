// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Characters/ShooterCharacter.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class GABESTOURNAMENT_API APlayerCharacter : public AShooterCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	TObjectPtr<USkeletalMeshComponent> FirstPersonMeshComponent;
	
	// ------ DEBUG ------

	UPROPERTY(EditDefaultsOnly, Category="Debug")
	bool ShowDebugInfo = true;

public:

	virtual void SetMeshColor(FColor Color) override;

	virtual void Die(FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	// ------ GETTER FUNCTIONS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Player Getters")
	UCameraComponent* GetFirstPersonCameraComponent() { return CameraComponent; }

	
};
