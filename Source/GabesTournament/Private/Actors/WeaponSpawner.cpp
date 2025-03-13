// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/WeaponSpawner.h"

#include "Actors/Weapons/Weapon.h"
#include "Components/ArrowComponent.h"

// Sets default values
AWeaponSpawner::AWeaponSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SpawnerArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	SpawnerArrowComponent->SetupAttachment(RootComponent);
	SpawnerArrowComponent->SetArrowSize(0.0f);

	WeaponEditorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Editor Mesh"));
	WeaponEditorMesh->SetupAttachment(SpawnerArrowComponent);
	WeaponEditorMesh->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

void AWeaponSpawner::OnConstruction(const FTransform& Transform)
{
	if (WeaponToSpawn)
	{
		WeaponEditorMesh->SetSkeletalMesh(WeaponToSpawn.GetDefaultObject()->GetMesh());
	}
}

// Called every frame
void AWeaponSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(SpawnedWeapon))
	{
		GEngine->AddOnScreenDebugMessage(99, 0.2f, FColor::Red, "SpawnedWeapon = " + SpawnedWeapon->GetName());
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
	else if (GetWorldTimerManager().TimerExists(SpawnTimer) == false)
	{		
		SpawnedWeapon = nullptr;
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AWeaponSpawner::SpawnWeapon, SpawnDelay, false);
		GEngine->AddOnScreenDebugMessage(100, SpawnDelay, FColor::Yellow, "Spawning Weapon");
	}
}

void AWeaponSpawner::SpawnWeapon()
{
	if (!WeaponToSpawn) return;
	SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponToSpawn);

	if (SpawnedWeapon == nullptr) return;

	SpawnedWeapon->SetIsPickup(true);
	SpawnedWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SpawnedWeapon->SetActorTransform(SpawnerArrowComponent->GetComponentTransform());
}

