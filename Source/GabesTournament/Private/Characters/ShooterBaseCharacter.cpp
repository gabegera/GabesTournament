// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShooterBaseCharacter.h"

#include "Components/HealthComponent.h"
#include "Components/InventoryComponent.h"

// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	
	WeaponChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	WeaponChildActorComponent->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryComponent->GetWeapons().IsEmpty() == false)
	{
		WeaponChildActorComponent->SetChildActorClass(InventoryComponent->GetWeapons().begin()->Get());
	}
	
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

