// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/ShooterPlayerController.h"

#include "Components/InventoryComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AShooterPlayerController::AShooterPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShooterPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	Input->BindAction(Input_Move, ETriggerEvent::Triggered, this, &AShooterPlayerController::Move);
	Input->BindAction(Input_Look, ETriggerEvent::Triggered, this, &AShooterPlayerController::Look);
	Input->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &AShooterPlayerController::Jump);
	Input->BindAction(Input_Dash, ETriggerEvent::Triggered, this, &AShooterPlayerController::Dash);
	
	Input->BindAction(Input_Fire, ETriggerEvent::Triggered, this, &AShooterPlayerController::UseWeapon);
	Input->BindAction(Input_SecondaryFire, ETriggerEvent::Triggered, this, &AShooterPlayerController::UseWeaponSecondary);
	
	Input->BindAction(Input_SwapWeapons, ETriggerEvent::Triggered, this, &AShooterPlayerController::SwapWeapons);
	Input->BindAction(Input_EquipSlot1, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipSlot1);
	Input->BindAction(Input_EquipSlot2, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipSlot2);
	Input->BindAction(Input_EquipSlot3, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipSlot3);
	Input->BindAction(Input_EquipSlot4, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipSlot4);
	Input->BindAction(Input_EquipSlot5, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipSlot5);
	Input->BindAction(Input_EquipSlot6, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipSlot6);
	Input->BindAction(Input_EquipSlot7, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipSlot7);
	Input->BindAction(Input_EquipSlot8, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipSlot8);
	Input->BindAction(Input_EquipSlot9, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipSlot9);
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

	// Run these functions to initialize their values.
	GetPlayerCharacter();
}

void AShooterPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorldTimerManager().IsTimerPaused(DashCooldownTimer) && GetPlayerCharacter()->GetMovementComponent()->IsFalling() == false)
	{
		GetWorldTimerManager().UnPauseTimer(DashCooldownTimer);
	}
}

// ------ MOVEMENT ------

void AShooterPlayerController::Move(const FInputActionInstance& Instance)
{
	GetPlayerCharacter()->AddMovementInput(GetPlayerCharacter()->GetActorRightVector(), Instance.GetValue().Get<FVector2D>().X);
	GetPlayerCharacter()->AddMovementInput(GetPlayerCharacter()->GetActorForwardVector(), Instance.GetValue().Get<FVector2D>().Y);
}

void AShooterPlayerController::Look(const FInputActionInstance& Instance)
{
	AddYawInput(Instance.GetValue().Get<FVector2D>().X * MouseLookSensitivity);
	AddPitchInput(Instance.GetValue().Get<FVector2D>().Y * MouseLookSensitivity);
	
}

void AShooterPlayerController::Jump()
{
	if (GetPlayerCharacter()->GetVelocity().Z < 0 && GetPlayerCharacter()->GetMovementComponent()->IsFalling()) return; 
	
	GetPlayerCharacter()->Jump();
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
		if (GetPlayerCharacter()->GetMovementComponent()->IsFalling() == false)
		{
			FVector DashForce =  FVector::ZeroVector;
			DashForce += GetPlayerCharacter()->GetActorRightVector() * XInput * DashLaunchForce.X;
			DashForce += GetPlayerCharacter()->GetActorForwardVector() * YInput * DashLaunchForce.Y;
			DashForce += GetPlayerCharacter()->GetActorUpVector() * DashLaunchForce.Z;

			GetPlayerCharacter()->LaunchCharacter(DashForce, true, true);
			
			if (HasAuthority())
			{
				// ON SERVER
				GetPlayerCharacter()->LaunchCharacter(DashForce, true, true);
			}
			else
			{
				// ON CLIENT
				Server_Dash(DashForce); 
			}
			
			GetWorldTimerManager().SetTimer(DashCooldownTimer, DashCooldown, false);
			GetWorldTimerManager().PauseTimer(DashCooldownTimer); // Timer is waiting for the player to land on the ground again.
		}
		
	}
	
}

void AShooterPlayerController::UseWeapon()
{
	Execute_Fire(GetPlayerCharacter()->GetWeaponChildActorComponent()->GetChildActor());	
}

void AShooterPlayerController::UseWeaponSecondary()
{
	Execute_SecondaryFire(GetPlayerCharacter()->GetWeaponChildActorComponent()->GetChildActor());
}


void AShooterPlayerController::SwapWeapons(const FInputActionInstance& Instance)
{	
	UInventoryComponent* Inventory = GetPlayerCharacter()->GetInventoryComponent();
	if (Inventory->GetWeapons().IsEmpty()) return;
	AWeaponActor* EquippedWeapon = GetEquippedWeapon();
	
	// If Scrolling Down, Equip the Next Weapon
	EWeaponSortingMethod SortingMethod = EWeaponSortingMethod::Ascending;

	// If Scrolling Up, Equip the Previous Weapon
	if (Instance.GetValue().Get<float>() > 0) SortingMethod = EWeaponSortingMethod::Descending;

	Inventory->SortWeapons(SortingMethod);

	TArray<TSubclassOf<AWeaponActor>> WeaponsArray = Inventory->GetWeapons().Array();
	
	for (int i = 0; i < WeaponsArray.Num(); i++)
	{
		if (WeaponsArray[i] == EquippedWeapon->GetClass())
		{
			if (i == WeaponsArray.Num() - 1)
			{
				Server_EquipWeapon(WeaponsArray[0]);
			}
			else Server_EquipWeapon(WeaponsArray[i + 1]);
			
			Inventory->SortWeapons(EWeaponSortingMethod::Ascending);
			return;
		}
	}
}

void AShooterPlayerController::EquipWeaponFromSlot(EWeaponSlot WeaponSlot)
{
	UInventoryComponent* Inventory = GetPlayerCharacter()->GetInventoryComponent();
	if (Inventory->GetWeapons().IsEmpty()) return;
	AWeaponActor* EquippedWeapon = GetEquippedWeapon();

	for (TSubclassOf<AWeaponActor>& Weapon : Inventory->GetWeapons())
	{
		if (Weapon.GetDefaultObject()->GetWeaponSlot() == WeaponSlot && Weapon != EquippedWeapon->GetClass())
		{
			Server_EquipWeapon(Weapon);
			return;
		}
	}
}

void AShooterPlayerController::Server_EquipWeapon_Implementation(TSubclassOf<AWeaponActor> Weapon)
{
	Client_EquipWeapon(Weapon);
}

void AShooterPlayerController::Client_EquipWeapon_Implementation(TSubclassOf<AWeaponActor> Weapon)
{
	GetPlayerCharacter()->GetWeaponChildActorComponent()->SetChildActorClass(Weapon);
}

void AShooterPlayerController::Server_Dash_Implementation(FVector DashForce)
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, "Server_Dash_Implementation CALLED");
	GetPlayerCharacter()->LaunchCharacter(DashForce, true, true);
}



