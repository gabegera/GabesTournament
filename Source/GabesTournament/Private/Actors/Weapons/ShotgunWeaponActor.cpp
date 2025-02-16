// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/ShotgunWeaponActor.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
AShotgunWeaponActor::AShotgunWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShotgunWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShotgunWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShotgunWeaponActor::Fire_Implementation()
{
	if (GetWorldTimerManager().IsTimerActive(FireRateTimer)) return;
	
	GEngine->AddOnScreenDebugMessage(10, 0.2f, FColor::Orange, "Weapon Fired");

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetParentActor());
	UCameraComponent* Camera = Player->GetFirstPersonCameraComponent();

	FVector ShotStart = Camera->GetComponentLocation();

	for (int i = 0; i < ShotCount; i++)
	{
		FVector ShotTarget = Camera->GetComponentLocation();
		ShotTarget += UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Camera->GetForwardVector(), BulletSpread / 2) * 100000.0f;

		Server_Fire(ShotStart, ShotTarget, Player);		
	}

	GetWorldTimerManager().SetTimer(FireRateTimer, GetFireRateInSeconds(), false);
}

void AShotgunWeaponActor::SecondaryFire_Implementation()
{
	
}

