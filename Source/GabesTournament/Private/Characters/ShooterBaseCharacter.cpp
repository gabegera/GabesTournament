// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShooterBaseCharacter.h"

#include "Components/HealthActorComponent.h"
#include "Components/InventoryComponent.h"

// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthActorComponent>(TEXT("Health"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	
	WeaponChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	WeaponChildActorComponent->SetupAttachment(GetMesh());
	WeaponChildActorComponent->SetHiddenInGame(true);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryComponent->GetWeapons().IsEmpty() == false)
	{
		WeaponChildActorComponent->SetChildActorClass(InventoryComponent->GetWeapons().begin()->Get());
	}

	Server_OwnerOnlySeeWeapon(OwnerOnlySeeWeapon);
}

float AShooterBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	HealthComponent->TakeDamage(DamageAmount);
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


// Called every frame
void AShooterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterBaseCharacter::Server_OwnerOnlySeeWeapon_Implementation(bool isTrue)
{
	Multicast_IsOtherWeaponsHidden(isTrue);
	Client_IsOwnerWeaponHidden(!isTrue);
}

void AShooterBaseCharacter::Multicast_IsOtherWeaponsHidden_Implementation(bool isHidden)
{
	WeaponChildActorComponent->SetHiddenInGame(isHidden);
}


void AShooterBaseCharacter::Client_IsOwnerWeaponHidden_Implementation(bool isHidden)
{
	WeaponChildActorComponent->SetHiddenInGame(isHidden);
}


