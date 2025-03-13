// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/Weapon.h"

#include "KismetTraceUtils.h"
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
	if (!isPickup || !OtherActor->Implements<UWeaponInterface>()) return;
	
	if (Execute_PickupWeapon(OtherActor, GetClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, OtherActor->GetName() + " Picked Up " + GetWeaponData().DisplayName);
		Destroy();
	}
}

bool AWeapon::GetCanFire(const EPrimaryOrSecondary PrimaryOrSecondaryFire)
{
	if (GetWorldTimerManager().IsTimerActive(BurstTimer)) return false;
	
	switch (PrimaryOrSecondaryFire)
	{
	case Primary:
		if (GetCharacterOwner()->GetInventoryComponent()->GetAmmo(GetWeaponData().PrimaryAmmoType) <= 0) return false;
		if (!GetWeaponData().isPrimaryFireAutomatic && hasPrimarySemiAutoFired) return false;
		if (GetWeaponData().DoesPrimaryOverheat && isOverheated) return false;
		if (GetWorldTimerManager().IsTimerActive(PrimaryFireRateTimer)) return false;
		if (!GetWeaponData().CanShootPrimaryAndSecondarySimultaneously && isSecondaryTriggerPulled) return false;
		if (GetWeaponData().DoesPrimaryCharge && PrimaryCurrentCharge < GetWeaponData().PrimaryTimeToCharge) return false;
	
		break;
	
	case Secondary:
		if (GetCharacterOwner()->GetInventoryComponent()->GetAmmo(GetWeaponData().SecondaryAmmoType) <= 0) return false;
		if (!GetWeaponData().isSecondaryFireAutomatic && hasSecondarySemiAutoFired) return false;
		if (GetWeaponData().DoesSecondaryOverheat && isOverheated) return false;
		if (GetWorldTimerManager().IsTimerActive(SecondaryFireRateTimer)) return false;
		if (!GetWeaponData().CanShootPrimaryAndSecondarySimultaneously && isPrimaryTriggerPulled) return false;
		if (GetWeaponData().DoesSecondaryCharge && SecondaryCurrentCharge < GetWeaponData().SecondaryTimeToCharge) return false;
	
		break;
	}
	
	return true;
}

FVector AWeapon::GetShotTargetWithSpread(float Spread)
{
	UCameraComponent* Camera = GetCharacterOwner()->GetFirstPersonCameraComponent();
	
	FVector ShotTarget = Camera->GetComponentLocation();
	ShotTarget += UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Camera->GetForwardVector(), Spread / 2) * 100000.0f;

	return ShotTarget;
}

void AWeapon::Fire_Implementation()
{
	isPrimaryTriggerPulled = true;
	
	ChargeWeapon(Primary);
	
	if (!GetCanFire(Primary)) return;
	
	const TSubclassOf<AProjectile> Projectile = GetWeaponData().PrimaryFireProjectile.LoadSynchronous();
	
	ShootWeapon(Projectile, GetWeaponData().PrimaryAmmoType, GetWeaponData().PrimaryAmmoCost, GetWeaponData().PrimaryFireDamage,
			GetWeaponData().PrimaryKnockbackForceMultiplier, GetWeaponData().PrimaryBurstCount, 
			GetWeaponData().PrimaryShotgunPelletCount, GetWeaponData().PrimaryFireBulletSpread, PrimaryFireRateTimer, ConvertFireRateToSeconds(GetWeaponData().PrimaryFireRate),
			GetWeaponData().DoesPrimaryOverheat);

	if (!GetWeaponData().isPrimaryFireAutomatic)
	{
		PrimaryCurrentCharge = 0.0f;
	}
		
	if (!GetWeaponData().isPrimaryFireAutomatic && (!isSecondaryTriggerPulled || !GetWeaponData().DoesSecondaryOverheat)
		&& CurrentOverheatValue < GetWeaponData().OverheatThreshold) isCoolingDown = true;

	if (GetWeaponData().isPrimaryFireAutomatic == false) hasPrimarySemiAutoFired = true;
}

void AWeapon::ReleasePrimaryTrigger_Implementation()
{
	isPrimaryTriggerPulled = false;

	GetWorldTimerManager().ClearTimer(PrimaryFireRateTimer);

	if (CurrentOverheatValue < GetWeaponData().OverheatThreshold && (!isSecondaryTriggerPulled || !GetWeaponData().DoesSecondaryOverheat))
	{
		GetWorldTimerManager().SetTimer(CooldownDelayTimer, this, &AWeapon::StartCooldown, GetWeaponData().CooldownDelay);
	}

	hasPrimarySemiAutoFired = false;
}

void AWeapon::SecondaryFire_Implementation()
{
	isSecondaryTriggerPulled = true;
	
	ChargeWeapon(Secondary);
	
	if (!GetCanFire(Secondary)) return;
	
	const TSubclassOf<AProjectile> Projectile = GetWeaponData().SecondaryFireProjectile.LoadSynchronous();
	
	ShootWeapon(Projectile, GetWeaponData().SecondaryAmmoType, GetWeaponData().SecondaryAmmoCost, GetWeaponData().SecondaryFireDamage,
			GetWeaponData().SecondaryKnockbackForceMultiplier, GetWeaponData().SecondaryBurstCount,
			GetWeaponData().SecondaryShotgunPelletCount, GetWeaponData().SecondaryFireBulletSpread, SecondaryFireRateTimer, ConvertFireRateToSeconds(GetWeaponData().SecondaryFireRate),
			GetWeaponData().DoesSecondaryOverheat);

	if (!GetWeaponData().isSecondaryFireAutomatic)
	{
		SecondaryCurrentCharge = 0.0f;
	}
			
	if (!GetWeaponData().isSecondaryFireAutomatic && (!isPrimaryTriggerPulled  || !GetWeaponData().DoesSecondaryOverheat)
		&& CurrentOverheatValue < GetWeaponData().OverheatThreshold) isCoolingDown = true;

	if (GetWeaponData().isSecondaryFireAutomatic == false) hasSecondarySemiAutoFired = true;
}

void AWeapon::ReleaseSecondaryTrigger_Implementation()
{
	isSecondaryTriggerPulled = false;

	GetWorldTimerManager().ClearTimer(SecondaryFireRateTimer);

	if (CurrentOverheatValue < GetWeaponData().OverheatThreshold && (!isPrimaryTriggerPulled || !GetWeaponData().DoesSecondaryOverheat))
	{
		GetWorldTimerManager().SetTimer(CooldownDelayTimer, this, &AWeapon::StartCooldown, GetWeaponData().CooldownDelay);
	}

	hasSecondarySemiAutoFired = false;
}


void AWeapon::ChargeWeapon(EPrimaryOrSecondary PrimaryOrSecondary)
{
	if (isOverheated) return;
	
	switch (PrimaryOrSecondary)
	{
	case Primary:	
		if (SecondaryCurrentCharge > 0) return;
		if (!GetWeaponData().isPrimaryFireAutomatic && hasPrimarySemiAutoFired) return;
		PrimaryCurrentCharge += GetWorld()->DeltaTimeSeconds;
		if (PrimaryCurrentCharge > GetWeaponData().PrimaryTimeToCharge) 
			PrimaryCurrentCharge = GetWeaponData().PrimaryTimeToCharge;
		break;

	case Secondary:	
		if (PrimaryCurrentCharge > 0) return;
		if (!GetWeaponData().isSecondaryFireAutomatic && hasSecondarySemiAutoFired) return;
		SecondaryCurrentCharge += GetWorld()->DeltaTimeSeconds;
		if (SecondaryCurrentCharge > GetWeaponData().SecondaryTimeToCharge) 
			SecondaryCurrentCharge = GetWeaponData().SecondaryTimeToCharge;
		break;
	}
}

void AWeapon::ShootWeapon(TSubclassOf<AProjectile> Projectile, EAmmoType AmmoType, int32 AmmoCost, float Damage, float KnockbackForceMultiplier, int32 BurstCount,
	int32 ShotgunPelletCount, float SpreadInDegrees, FTimerHandle& FireRateTimerHandle, float FireRateInSeconds, bool DoesOverheat)
{
	// For Every shotgun pellet fire a shot.
	for (int32 i = 0; i < ShotgunPelletCount; i++)
	{
		FVector ShotStart = GetCharacterOwner()->GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector ShotTarget = GetShotTargetWithSpread(SpreadInDegrees);
		
		if (Projectile.GetDefaultObject() != nullptr)
		{
			FHitResult Hit;
			FCollisionQueryParams IgnorePlayer;
			IgnorePlayer.AddIgnoredActor(GetCharacterOwner()->GetUniqueID());
			
			GetWorld()->LineTraceSingleByChannel(Hit, ShotStart, ShotTarget, ECC_WorldDynamic, IgnorePlayer);

			ShotStart = MuzzleArrowComponent->GetComponentLocation();
			// ShotTarget = Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;
			
			Server_ShootProjectile(Projectile, Damage, KnockbackForceMultiplier, ShotStart, ShotTarget, GetCharacterOwner());
		}
		else
		{
			Server_ShootHitscan(Damage, KnockbackForceMultiplier, ShotStart, ShotTarget, GetCharacterOwner());
		}
	}

	// Burst Fire
	if (BurstCount > 1)
	{
    	FTimerDelegate BurstDelegate;
		BurstDelegate.BindUFunction(this, FName("ShootWeapon"),
			Projectile, AmmoType, Damage, KnockbackForceMultiplier, BurstCount,
			ShotgunPelletCount, SpreadInDegrees, FireRateTimerHandle, FireRateInSeconds, DoesOverheat);

		CurrentBurstCount++;

		if (GetWorldTimerManager().IsTimerActive(BurstTimer) == false)
		{
			GetWorldTimerManager().SetTimer(BurstTimer, BurstDelegate, FireRateInSeconds, true);
		}
		else if (CurrentBurstCount >= BurstCount)
		{
			GetWorldTimerManager().ClearTimer(BurstTimer);
			CurrentBurstCount = 0;
		}
	}
	else
	{
		GetWorldTimerManager().SetTimer(FireRateTimerHandle, FireRateInSeconds, false);
	}

	// Overheating
	if (DoesOverheat)
    {
		if (GetWorldTimerManager().IsTimerActive(CooldownDelayTimer)) GetWorldTimerManager().ClearTimer(CooldownDelayTimer);
		
    	if (CurrentOverheatValue + FireRateInSeconds > GetWeaponData().OverheatThreshold)
    	{
    		CurrentOverheatValue = GetWeaponData().OverheatThreshold;
    	}
    	else
    	{
    		CurrentOverheatValue += FireRateInSeconds;
    	}
    	
    	isCoolingDown = false;

    	if (CurrentOverheatValue >= GetWeaponData().OverheatThreshold)
    	{
    		isOverheated = true;
    		GetWorldTimerManager().SetTimer(CooldownDelayTimer, this, &AWeapon::StartCooldown, GetWeaponData().OverheatedCooldownDelay, false);
    	}
    }

	GetCharacterOwner()->GetInventoryComponent()->RemoveAmmo(AmmoType, AmmoCost);
	
	GEngine->AddOnScreenDebugMessage(10, 0.2f, FColor::Orange, "Weapon Fired");
	
}

void AWeapon::Server_ShootHitscan_Implementation(float Damage, float KnockbackForceMultiplier, FVector Start, FVector End, ACharacter* Causer)
{
	FCollisionQueryParams IgnorePlayer;
	IgnorePlayer.AddIgnoredActor(Causer->GetUniqueID());
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldDynamic, IgnorePlayer);
	DrawDebugLineTraceSingle(GetWorld(), MuzzleArrowComponent->GetComponentLocation(), Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd, EDrawDebugTrace::ForDuration, true, FHitResult(), FColor::Red, FColor::Green, 0.2f);

	if (Hit.GetActor() && Hit.GetActor()->Implements<UDamageInterface>())
	{
		GEngine->AddOnScreenDebugMessage(12, 0.2f, FColor::Purple, "Hit Actor: " + Hit.GetActor()->GetName());
		
		Execute_DealDamage(Hit.GetActor(), Damage, FPointDamageEvent(), Causer->GetController(), Causer);
		Execute_AddImpulseAtLocation(Hit.GetActor(), UKismetMathLibrary::GetDirectionUnitVector(Hit.TraceStart, Hit.TraceEnd) * KnockbackForceMultiplier * Damage, Hit.ImpactPoint, NAME_None);
	}

	// Multicast_ShootHitscan(Damage, MuzzleArrowComponent->GetComponentLocation(), Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd, Causer);
}

// void AWeapon::Multicast_ShootHitscan_Implementation(float Damage, FVector Start, FVector End, ACharacter* Causer)
// {
// 	FCollisionQueryParams IgnorePlayer;
// 	IgnorePlayer.AddIgnoredActor(Causer->GetUniqueID());
// 	
// 	FHitResult Hit;
// 	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldDynamic, IgnorePlayer);
// 	DrawDebugLineTraceSingle(GetWorld(), Start, End, EDrawDebugTrace::ForDuration, true, FHitResult(), FColor::Red, FColor::Green, 0.2f);
//
// 	if (Hit.GetActor())
// 	{
// 		GEngine->AddOnScreenDebugMessage(12, 0.2f, FColor::Purple, "Hit Actor: " + Hit.GetActor()->GetName());
// 		Hit.GetActor()->LoseHealth(Damage, FPointDamageEvent(), Causer->GetController(), Causer);
//
// 		// TODO: Add Physics Impacts.
// 	}
// }

void AWeapon::Server_ShootProjectile_Implementation(TSubclassOf<AProjectile> ProjectileActor, float Damage, float KnockbackForceMultiplier, FVector Start, FVector Target, ACharacter* Causer)
{
	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileActor);

	if (SpawnedProjectile == nullptr) return;

	SpawnedProjectile->SetActorLocation(Start);
	SpawnedProjectile->GetProjectileMovement()->Velocity = UKismetMathLibrary::FindLookAtRotation(Start, Target).Vector() * SpawnedProjectile->GetProjectileMovement()->InitialSpeed;
	SpawnedProjectile->SetSpawner(Causer);
	SpawnedProjectile->SetImpactDamage(Damage);
	SpawnedProjectile->SetKnockbackForceMultiplier(KnockbackForceMultiplier);

	// Multicast_ShootProjectile(ProjectileActor, Damage, Start, Target, Causer);
}

void AWeapon::Multicast_ShootProjectile_Implementation(TSubclassOf<AProjectile> ProjectileActor, float Damage, FVector Start, FVector Target, ACharacter* Causer)
{
	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileActor);

	if (SpawnedProjectile == nullptr) return;

	SpawnedProjectile->SetActorLocation(Start);
	SpawnedProjectile->GetProjectileMovement()->Velocity = UKismetMathLibrary::FindLookAtRotation(Start, Target).Vector() * SpawnedProjectile->GetProjectileMovement()->InitialSpeed;
	SpawnedProjectile->SetSpawner(Causer);
	SpawnedProjectile->SetImpactDamage(Damage);
	SpawnedProjectile->SetKnockbackForceMultiplier(Damage);
}

void AWeapon::StartCooldown()
{
	isCoolingDown = true;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isPickup)
	{
		SetActorRotation(GetActorRotation() + FRotator(0, 1, 0));
	}

	if (GetWeaponData().DoesPrimaryOverheat && CurrentOverheatValue <= 0)
	{
		CurrentOverheatValue = 0;
		isCoolingDown = false;
		isOverheated = false;
	}
	else if (GetWeaponData().DoesPrimaryOverheat && isCoolingDown)
	{
		CurrentOverheatValue -= DeltaTime * GetWeaponData().CooldownSpeedMultiplier;
	}

	if (GetWeaponData().DoesPrimaryOverheat)
	{
		GEngine->AddOnScreenDebugMessage(400, 1.0f, FColor::White, "CurrentOverheatValue: " + FString::SanitizeFloat(CurrentOverheatValue));
	}

	if (GetWeaponData().DoesPrimaryCharge && PrimaryCurrentCharge > 0.0f && !isPrimaryTriggerPulled)
	{
		PrimaryCurrentCharge -= DeltaTime * GetWeaponData().PrimaryDeChargeMultiplier;
	}
	else if (PrimaryCurrentCharge < 0.0f && !isPrimaryTriggerPulled)
	{
		PrimaryCurrentCharge = 0.0f;
	}

	if (GetWeaponData().DoesSecondaryCharge && SecondaryCurrentCharge > 0.0f && !isSecondaryTriggerPulled)
	{
		SecondaryCurrentCharge -= DeltaTime * GetWeaponData().SecondaryDeChargeMultiplier;
	}
	else if (SecondaryCurrentCharge < 0.0f && !isSecondaryTriggerPulled)
	{
		SecondaryCurrentCharge = 0.0f;
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

FWeaponData AWeapon::GetWeaponData()
{
	if (WeaponDataTable.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon: %s has null WeaponData."), *this->GetName());
		return FWeaponData();
	}
	
	return *WeaponDataTable.GetRow<FWeaponData>("");
}

float AWeapon::GetFireRateInSeconds(EPrimaryOrSecondary PrimaryOrSecondary)
{
	switch (PrimaryOrSecondary)
	{
	case Primary:
		return ConvertFireRateToSeconds(GetWeaponData().PrimaryFireRate);

	case Secondary:
		return ConvertFireRateToSeconds(GetWeaponData().SecondaryFireRate);

	default:
		return 0.0f;
	}
}

float AWeapon::GetCurrentCharge(EPrimaryOrSecondary PrimaryOrSecondary)
{
	switch (PrimaryOrSecondary)
	{
	case Primary:
		return PrimaryCurrentCharge;

	case Secondary:
		return SecondaryCurrentCharge;

	default:
		return 0.0f;
	}
}

float AWeapon::GetMaxCharge(EPrimaryOrSecondary PrimaryOrSecondary)
{
	switch (PrimaryOrSecondary)
	{
	case Primary:
		return GetWeaponData().PrimaryTimeToCharge;

	case Secondary:
		return GetWeaponData().SecondaryTimeToCharge;

	default:
		return 0.0f;
	}
}

