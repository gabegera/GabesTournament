// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WeaponActor.h"
#include "Actors/ProjectileActor.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponDataTableRow.DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("Content/DataTables/DT_Weapons")));

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>("MuzzleArrow");
	MuzzleArrow->SetupAttachment(SkeletalMeshComponent);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponActor::OnConstruction(const FTransform& Transform)
{
	
}


// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponActor::Shoot(FVector StartingPosition, FVector Target, float SpreadInDegrees, float Damage, float Range, EAmmoType AmmoType, AProjectileActor* ProjectileActor)
{
	if (CanShoot() == false) return;
 
	// Shoots for every pellet the weapon has. Will only fire once if weapon is not a shotgun.
	for (int i = 0; i < GetWeaponData().ShotgunProjectileCount; i++)
	{
		if (ProjectileActor)
		{
			ShootProjectile(ProjectileActor, StartingPosition, Target, SpreadInDegrees, Damage);
		}
		else
		{
        	ShootHitscan(StartingPosition, Target, SpreadInDegrees, Damage);
        }
 
		// If the Weapon is not a shotgun break the loop.
		if (GetWeaponData().IsShotgun == false) break;
	}

	GetWorldTimerManager().SetTimer(FireRateTimer, GetWeaponData().FireRate, false);
	
	// if (IsAiming())
	// {
	// 	AddRecoil(GetAimingRecoil());
	// }
	// else
	// AddRecoil(GetHipfireRecoil());
}

void AWeaponActor::Shoot(FVector StartingPosition, FVector Target, FWeaponData WeaponData)
{
	if (CanShoot() == false) return;
 
	// Shoots for every pellet the weapon has. Will only fire once if weapon is not a shotgun.
	for (int i = 0; i < GetWeaponData().ShotgunProjectileCount; i++)
	{
		if (WeaponData.ProjectileActor)
		{
			AProjectileActor* LoadedProjectile = WeaponData.ProjectileActor.LoadSynchronous();
			ShootProjectile(LoadedProjectile, StartingPosition, Target, WeaponData.WeaponSpreadInDegrees, WeaponData.Damage);
		}
		else
		{
			ShootHitscan(StartingPosition, Target, WeaponData.WeaponSpreadInDegrees, WeaponData.Damage);
		}
 
		// If the Weapon is not a shotgun break the loop.
		if (GetWeaponData().IsShotgun == false) break;
	}

	GetWorldTimerManager().SetTimer(FireRateTimer, GetWeaponData().FireRate, false);
}

void AWeaponActor::ShootHitscan(FVector StartingPosition, FVector Target, float SpreadInDegrees, float Damage)
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, StartingPosition, Target, ECC_WorldDynamic);
	DrawDebugLine(GetWorld(), StartingPosition, Target, FColor::Red, false, 0.25, 0, 1.0f);

	Hit.GetActor()->TakeDamage(Damage, FDamageEvent(), GetParentActor()->GetInstigatorController(), GetParentActor());
}

void AWeaponActor::ShootProjectile(AProjectileActor* ProjectileActor, FVector StartingPosition, FVector Target, float SpreadInDegrees, float Damage)
{
	
}

void AWeaponActor::UseSecondaryAction()
{
	
}

bool AWeaponActor::CanShoot()
{
	return true;
}

