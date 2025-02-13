// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Characters/ShooterBaseCharacter.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class GABESTOURNAMENT_API APlayerCharacter : public AShooterBaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UCameraComponent* CameraComponent;

	// ------ DEBUG ------

	UPROPERTY(EditDefaultsOnly, Category="Debug")
	bool ShowDebugInfo = true;
	
};
