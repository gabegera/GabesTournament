// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WeaponSpawner.h"
#include "Actors/WeaponActor.h"


// Sets default values
AWeaponSpawner::AWeaponSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMesh"));
	SpawnerMesh->SetupAttachment(RootComponent);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(SpawnerMesh);
	WeaponMesh->SetRelativeLocation(FVector(0, 0, 60));
}

// Called when the game starts or when spawned
void AWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Weapon != nullptr)
	{
		if (Weapon.GetDefaultObject()->GetSkeletalMesh()) WeaponMesh->SetSkinnedAsset(Weapon.GetDefaultObject()->GetSkeletalMesh());
	}
	
}

// Called every frame
void AWeaponSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

