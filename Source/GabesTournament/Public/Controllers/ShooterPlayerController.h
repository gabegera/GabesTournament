// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Characters/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "ShooterPlayerController.generated.h"

UCLASS()
class GABESTOURNAMENT_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController();

protected:

	// ------ ENHANCED INPUT ------
	
	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpInputAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* DashInputAction;

	UFUNCTION(BlueprintCallable, Category="Input")
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	// ------ INPUT VARIABLES ------

	UPROPERTY(EditAnywhere, Category="Input")
	float MouseLookSensitivity = 0.2f;

	// ------ CHARACTER FUNCTIONS -------

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// ------ MOVEMENT ------

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Move(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Look(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Jump();

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Dash(const FInputActionInstance& Instance);

	// ------ MOVEMENT VARIABLES ------

	UPROPERTY(EditAnywhere, Category="Movement")
	FVector DashLaunchForce = FVector(800.0f, 800.0f, 600.0f);

	UPROPERTY(EditAnywhere, Category="Movement", meta=(Units="Seconds"))
	float DashCooldown = 0.5f;
	
	FTimerHandle DashCooldownTimer;
	
};
