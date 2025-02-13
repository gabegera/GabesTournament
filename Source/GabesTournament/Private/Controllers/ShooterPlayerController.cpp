// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/ShooterPlayerController.h"

#include "GameFramework/PawnMovementComponent.h"

AShooterPlayerController::AShooterPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShooterPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Move);
	Input->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Look);
	Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Jump);
	Input->BindAction(DashInputAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Dash);
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}

		SetupPlayerInputComponent(InputComponent);
	}
}

void AShooterPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorldTimerManager().IsTimerPaused(DashCooldownTimer) && GetCharacter()->GetMovementComponent()->IsFalling() == false)
	{
		GetWorldTimerManager().UnPauseTimer(DashCooldownTimer);
	}
}

// ------ MOVEMENT ------

void AShooterPlayerController::Move(const FInputActionInstance& Instance)
{
	GetCharacter()->AddMovementInput(GetCharacter()->GetActorRightVector(), Instance.GetValue().Get<FVector2D>().X);
	GetCharacter()->AddMovementInput(GetCharacter()->GetActorForwardVector(), Instance.GetValue().Get<FVector2D>().Y);
}

void AShooterPlayerController::Look(const FInputActionInstance& Instance)
{
	AddYawInput(Instance.GetValue().Get<FVector2D>().X * MouseLookSensitivity);
	AddPitchInput(Instance.GetValue().Get<FVector2D>().Y * MouseLookSensitivity);
	
}

void AShooterPlayerController::Jump()
{
	if (GetCharacter()->GetVelocity().Z < 0 && GetCharacter()->GetMovementComponent()->IsFalling()) return; 
	
	GetCharacter()->Jump();
}

void AShooterPlayerController::Dash(const FInputActionInstance& Instance)
{
	if (GetWorldTimerManager().IsTimerActive(DashCooldownTimer)) return;
	
	double XInput = Instance.GetValue().Get<FVector>().X;
	double YInput = Instance.GetValue().Get<FVector>().Y;
	double ZInput = Instance.GetValue().Get<FVector>().Z;

	// ZInput is used to check if the shift key was pressed. X and Y are the directions that the character will dash.
	// The Character has to be moving in order to dash.
	if (ZInput > 0 && (XInput != 0 || YInput != 0))
	{
		if (GetCharacter()->GetMovementComponent()->IsFalling() == false)
		{
			FVector DashForce =  FVector::ZeroVector;
			DashForce += GetCharacter()->GetActorRightVector() * XInput * DashLaunchForce.X;
			DashForce += GetCharacter()->GetActorForwardVector() * YInput * DashLaunchForce.Y;
			DashForce += GetCharacter()->GetActorUpVector() * DashLaunchForce.Z;

			GetCharacter()->LaunchCharacter(DashForce, true, true);

			GetWorldTimerManager().SetTimer(DashCooldownTimer, DashCooldown, false);
			GetWorldTimerManager().PauseTimer(DashCooldownTimer); // Timer is waiting for the player to land on the ground again.
		}
		
	}
	
}
