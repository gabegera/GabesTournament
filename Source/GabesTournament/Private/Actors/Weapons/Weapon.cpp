// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/Weapon.h"

#include "Actors/Projectile.h"
#include "Characters/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(RootComponent);

	MuzzleArrowComponent = CreateDefaultSubobject<UArrowComponent>("Muzzle");
	MuzzleArrowComponent->SetupAttachment(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	SphereComponent->SetupAttachment(MeshComponent);
	SphereComponent->SetAutoActivate(false);
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = GetCharacterOwner();

	if (isPickup) SphereComponent->SetActive(true);
	
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (!isPickup || !OtherActor->Implements<UWeaponInterface>()) return;
	
	// if (Execute_PickupWeapon(OtherActor, GetClass()))
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, OtherActor->GetName() + " Picked Up " + DisplayName);
	// 	Destroy();
	// }
}

bool AWeapon::GetCanFire(const EPrimaryOrSecondary PrimaryOrSecondaryFire)
{
	// if (GetWorldTimerManager().IsTimerActive(BurstTimer)) return false;
	//
	// switch (PrimaryOrSecondaryFire)
	// {
	// case Primary:
	// 	if (GetCharacterOwner()->GetInventoryComponent()->GetAmmo(GetWeaponData().PrimaryAmmoType) <= 0) return false;
	// 	if (!GetWeaponData().isPrimaryFireAutomatic && hasPrimarySemiAutoFired) return false;
	// 	if (GetWeaponData().DoesPrimaryOverheat && isOverheated) return false;
	// 	if (GetWorldTimerManager().IsTimerActive(FireRateTimer)) return false;
	// 	if (!GetWeaponData().CanShootPrimaryAndSecondarySimultaneously && isSecondaryTriggerPulled) return false;
	// 	if (GetWeaponData().DoesPrimaryCharge && PrimaryCurrentCharge < GetWeaponData().PrimaryTimeToCharge) return false;
	//
	// 	break;
	//
	// case Secondary:
	// 	if (GetCharacterOwner()->GetInventoryComponent()->GetAmmo(GetWeaponData().SecondaryAmmoType) <= 0) return false;
	// 	if (!GetWeaponData().isSecondaryFireAutomatic && hasSecondarySemiAutoFired) return false;
	// 	if (GetWeaponData().DoesSecondaryOverheat && isOverheated) return false;
	// 	if (GetWorldTimerManager().IsTimerActive(SecondaryFireRateTimer)) return false;
	// 	if (!GetWeaponData().CanShootPrimaryAndSecondarySimultaneously && isPrimaryTriggerPulled) return false;
	// 	if (GetWeaponData().DoesSecondaryCharge && SecondaryCurrentCharge < GetWeaponData().SecondaryTimeToCharge) return false;
	//
	// 	break;
	// }
	//
	return true;
}

FVector AWeapon::VectorWithSpreadFromPoints(FVector StartLocation, FVector EndLocation, float Spread)
{
	FVector Direction = EndLocation - StartLocation;
	Direction.Normalize();
	
	return StartLocation + UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Direction, Spread / 2) * FVector::Distance(StartLocation, EndLocation);
}

void AWeapon::PullPrimaryTrigger()
{
	Fire();
	isPrimaryTriggerPulled = true;
}

void AWeapon::ReleasePrimaryTrigger()
{
	isPrimaryTriggerPulled = false;
}

void AWeapon::PullSecondaryTrigger()
{
	SecondaryFire();
	isSecondaryTriggerPulled = true;
}

void AWeapon::ReleaseSecondaryTrigger()
{
	isSecondaryTriggerPulled = false;
}

bool AWeapon::ChargeWeapon(EPrimaryOrSecondary PrimaryOrSecondary, float TimeToCharge)
{
	if (isOverheated) return false;

	if (PrimaryOrSecondary == Primary)
	{
		if (SecondaryCurrentCharge > 0) return false;
		
		PrimaryCurrentCharge += GetWorld()->DeltaTimeSeconds;
        
        if (PrimaryCurrentCharge >= TimeToCharge)
        {
        	PrimaryCurrentCharge = TimeToCharge;
        	return true;
        }
	}

	if (PrimaryOrSecondary == Secondary)
	{
		if (PrimaryCurrentCharge > 0) return false;
		
		SecondaryCurrentCharge += GetWorld()->DeltaTimeSeconds;
        
		if (SecondaryCurrentCharge >= TimeToCharge)
		{
			SecondaryCurrentCharge = TimeToCharge;
			return true;
		}
	}

	return false;
}

void AWeapon::Server_ShootProjectile_Implementation(EPrimaryOrSecondary PrimaryOrSecondary, EAmmoType AmmoType, int32 AmmoCost, TSubclassOf<AProjectile> Projectile, int32 ProjectilesToSpawn, float ProjectileSpeed, 
float Damage, float KnockbackForceMultiplier, float SpreadInDegrees, bool isAutomatic, float FireRateRPM, bool DoesOverheat)
{
	if (GetWorldTimerManager().IsTimerActive(FireRateTimer)) return;

	switch (PrimaryOrSecondary)
	{
	case Primary:
		if (isPrimaryTriggerPulled && !isAutomatic) return;
	case Secondary:
		if (isSecondaryTriggerPulled && !isAutomatic) return;
	default:
		break;
	}

	for (int32 i = 0; i < ProjectilesToSpawn; i++)
	{
		FVector CameraLocation = GetCharacterOwner()->GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector ShotStart = CameraLocation;
		FVector ShotTarget = CameraLocation + GetCharacterOwner()->GetFirstPersonCameraComponent()->GetForwardVector() * 10000.0f;
		ShotTarget = VectorWithSpreadFromPoints(ShotStart, ShotTarget, SpreadInDegrees);

		FHitResult Hit;
		FCollisionQueryParams IgnorePlayer;
		IgnorePlayer.AddIgnoredActor(GetCharacterOwner()->GetUniqueID());
		GetWorld()->LineTraceSingleByChannel(Hit, ShotStart, ShotTarget, ECC_WorldDynamic, IgnorePlayer);

		ShotTarget = Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;

		AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile, MuzzleArrowComponent->GetComponentLocation(), GetActorForwardVector().ToOrientationRotator());

		FVector ShotDirection = Hit.TraceEnd - Hit.TraceStart;
		ShotDirection.Normalize();
		SpawnedProjectile->GetProjectileMovement()->Velocity = ShotDirection * ProjectileSpeed;
		SpawnedProjectile->SetSpawner(GetCharacterOwner());
		SpawnedProjectile->SetImpactDamage(Damage);
		SpawnedProjectile->SetKnockbackForceMultiplier(KnockbackForceMultiplier);
	}

	GetCharacterOwner()->GetInventoryComponent()->RemoveAmmo(AmmoType, AmmoCost);
	
	if (FireRateRPM > 0.0f) GetWorldTimerManager().SetTimer(FireRateTimer, ConvertFireRateToSeconds(FireRateRPM), false);
}

void AWeapon::Server_ShootProjectileBurst_Implementation(EPrimaryOrSecondary PrimaryOrSecondary, EAmmoType AmmoType, int32 AmmoCost, TSubclassOf<AProjectile> Projectile, int32 ProjectilesToSpawn, int32 BurstCount, float BurstDelay,
float ProjectileSpeed, float Damage, float KnockbackForceMultiplier, float SpreadInDegrees, bool isAutomatic, float FireRateRPM, bool DoesOverheat)
{	
	// Server_ShootProjectile(AmmoType, AmmoCost, Projectile, ProjectilesToSpawn, ProjectileSpeed, Damage, KnockbackForceMultiplier, SpreadInDegrees, FireRateRPM, DoesOverheat);
	// CurrentBurstCount++;
	//
	// if (CurrentBurstCount >= BurstCount)
	// {
	// 	CurrentBurstCount = 0;
	//
	// 	if (FireRateRPM > 0.0f) GetWorldTimerManager().SetTimer(FireRateTimer, ConvertFireRateToSeconds(FireRateRPM), false);
	//
	// 	return;
	// }
	//
	// FTimerDelegate BurstTimerDelegate;
	// BurstTimerDelegate.BindUFunction(this, FName("ShootProjectileBurst"), AmmoType, AmmoCost, Projectile, ProjectilesToSpawn, BurstCount, BurstDelay,
	// 							  ProjectileSpeed, Damage, KnockbackForceMultiplier, SpreadInDegrees, true, FireRateRPM, DoesOverheat);
	//
	// GetWorldTimerManager().SetTimer(BurstTimer, BurstTimerDelegate, BurstDelay, false);
}

void AWeapon::Server_ShootHitscan_Implementation(EPrimaryOrSecondary PrimaryOrSecondary, EAmmoType AmmoType, int32 AmmoCost, int32 ProjectilesToSpawn, float Damage, float KnockbackForceMultiplier, float SpreadInDegrees,
bool isAutomatic, float FireRateRPM, bool DoesOverheat)
{
	if (GetWorldTimerManager().IsTimerActive(FireRateTimer)) return;

	switch (PrimaryOrSecondary)
	{
	case Primary:
		if (isPrimaryTriggerPulled && !isAutomatic) return;
	case Secondary:
		if (isSecondaryTriggerPulled && !isAutomatic) return;
	default:
		break;
	}

	for (int32 i = 0; i < ProjectilesToSpawn; i++)
	{
		FVector CameraLocation = GetCharacterOwner()->GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector ShotStart = CameraLocation;
		FVector ShotTarget = CameraLocation + GetCharacterOwner()->GetFirstPersonCameraComponent()->GetForwardVector() * 10000.0f;
		ShotTarget = VectorWithSpreadFromPoints(ShotStart, ShotTarget, SpreadInDegrees);

		FHitResult Hit;
		FCollisionQueryParams IgnorePlayer;
		IgnorePlayer.AddIgnoredActor(GetCharacterOwner()->GetUniqueID());
		GetWorld()->LineTraceSingleByChannel(Hit, ShotStart, ShotTarget, ECC_WorldDynamic, IgnorePlayer);

		if (Hit.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), MuzzleArrowComponent->GetComponentLocation(), Hit.ImpactPoint, FColor::Green, false, 0.2f);
		}
		else
		{
			DrawDebugLine(GetWorld(), MuzzleArrowComponent->GetComponentLocation(), Hit.TraceEnd, FColor::Red, false, 0.2f);
		}
		
		if (IsValid(Hit.GetActor()) == false) break;

		Hit.GetActor()->TakeDamage(Damage, FPointDamageEvent(), GetCharacterOwner()->GetController(), GetCharacterOwner());

		FVector HitDirection = Hit.TraceEnd - Hit.TraceStart;
		HitDirection.Normalize();
		
		if (Hit.GetComponent()->GetClass() == UStaticMeshComponent::StaticClass())
		{
			UStaticMeshComponent* SMComponent = Cast<UStaticMeshComponent>(Hit.GetComponent());

			if (SMComponent->IsSimulatingPhysics())
			{
				SMComponent->AddImpulseAtLocation(HitDirection * Damage * KnockbackForceMultiplier, Hit.ImpactPoint);
			}
		}
		else if (Hit.Component->GetClass() == USkeletalMeshComponent::StaticClass())
		{
			USkeletalMeshComponent* SKMComponent = Cast<USkeletalMeshComponent>(Hit.GetComponent());

			if (SKMComponent->IsSimulatingPhysics())
			{
				SKMComponent->AddImpulseAtLocation(HitDirection * Damage * KnockbackForceMultiplier, Hit.ImpactPoint);
			}
		}		
	}

	CharacterOwner->GetInventoryComponent()->RemoveAmmo(AmmoType, AmmoCost);

	GetWorldTimerManager().SetTimer(FireRateTimer, ConvertFireRateToSeconds(FireRateRPM), false);
}

void AWeapon::Server_ShootHitscanBurst_Implementation(EPrimaryOrSecondary PrimaryOrSecondary, EAmmoType AmmoType, int32 AmmoCost, int32 ProjectilesToSpawn, int32 BurstCount, float BurstDelay,
float Damage, float KnockbackForceMultiplier, float SpreadInDegrees, bool isAutomatic, float FireRateRPM, bool DoesOverheat)
{
	// if (GetWorldTimerManager().IsTimerActive(BurstTimer) && CurrentBurstCount > 0) return;
	//
	// CurrentBurstCount++;
	//
	// if (CurrentBurstCount >= BurstCount)
	// {
	// 	CurrentBurstCount = 0;
	//
	// 	if (FireRateRPM > 0.0f) GetWorldTimerManager().SetTimer(FireRateTimer, ConvertFireRateToSeconds(FireRateRPM), false);
	//
	// 	return;
	// }
	//
	// FTimerDelegate BurstTimerDelegate;
	// BurstTimerDelegate.BindUObject(this, &AWeapon::Server_ShootHitscan, AmmoType, AmmoCost, ProjectilesToSpawn, Damage, KnockbackForceMultiplier,
	// 							   SpreadInDegrees, true, 0.0f, DoesOverheat);
	//
	// GetWorldTimerManager().SetTimer(BurstTimer, BurstTimerDelegate, BurstDelay, false);
}

void AWeapon::StartCooldown()
{
	isCoolingDown = true;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isPrimaryTriggerPulled == false)
	{
		PrimaryCurrentCharge -= DeltaTime;
		if (PrimaryCurrentCharge < 0) PrimaryCurrentCharge = 0;
	}

	if (isSecondaryTriggerPulled == false)
	{
		SecondaryCurrentCharge -= DeltaTime;
		if (SecondaryCurrentCharge < 0) SecondaryCurrentCharge = 0;
	}

	if (isPickup)
	{
		SetActorRotation(GetActorRotation() + FRotator(0, 1, 0));
	}

	FString PrimaryString = "isPrimaryTriggerPulled: ";
	PrimaryString.Append(isPrimaryTriggerPulled ? "true" : "false");
	GEngine->AddOnScreenDebugMessage(401, 1.0f, FColor::White, PrimaryString);
	FString SecondaryString = "isSecondaryTriggerPulled: ";
	SecondaryString.Append(isSecondaryTriggerPulled ? "true" : "false");
	GEngine->AddOnScreenDebugMessage(402, 1.0f, FColor::White, SecondaryString);

	FString PrimaryChargeString = "PrimaryCharge: ";
	PrimaryChargeString.Append(FString::SanitizeFloat(PrimaryCurrentCharge));
	GEngine->AddOnScreenDebugMessage(403, 1.0f, FColor::White, PrimaryChargeString);
	FString SecondaryChargeString = "SecondaryCharge: ";
	SecondaryChargeString.Append(FString::SanitizeFloat(SecondaryCurrentCharge));
	GEngine->AddOnScreenDebugMessage(404, 1.0f, FColor::White, SecondaryChargeString);
}

// ------ GETTERS ------

APlayerCharacter* AWeapon::GetCharacterOwner()
{
	return CharacterOwner ? CharacterOwner : Cast<APlayerCharacter>(GetParentActor());
}

// float AWeapon::GetFireRateInSeconds(EPrimaryOrSecondary PrimaryOrSecondary)
// {
// 	switch (PrimaryOrSecondary)
// 	{
// 	case Primary:
// 		return ConvertFireRateToSeconds(GetWeaponData().PrimaryFireRate);
//
// 	case Secondary:
// 		return ConvertFireRateToSeconds(GetWeaponData().SecondaryFireRate);
//
// 	default:
// 		return 0.0f;
// 	}
// }

// float AWeapon::GetCurrentCharge(EPrimaryOrSecondary PrimaryOrSecondary)
// {
// 	switch (PrimaryOrSecondary)
// 	{
// 	case Primary:
// 		return PrimaryCurrentCharge;
//
// 	case Secondary:
// 		return SecondaryCurrentCharge;
//
// 	default:
// 		return 0.0f;
// 	}
// }

// float AWeapon::GetMaxCharge(EPrimaryOrSecondary PrimaryOrSecondary)
// {
// 	switch (PrimaryOrSecondary)
// 	{
// 	case Primary:
// 		return GetWeaponData().PrimaryTimeToCharge;
//
// 	case Secondary:
// 		return GetWeaponData().SecondaryTimeToCharge;
//
// 	default:
// 		return 0.0f;
// 	}
// }

