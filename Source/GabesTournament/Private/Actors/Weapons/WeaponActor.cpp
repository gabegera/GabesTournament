// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/WeaponActor.h"

#include "KismetTraceUtils.h"
#include "Characters/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Engine/DamageEvents.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(GetRootComponent());

	MuzzleArrowComponent = CreateDefaultSubobject<UArrowComponent>("Muzzle");
	MuzzleArrowComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponActor::Fire_Implementation()
{
	if (GetWorldTimerManager().IsTimerActive(FireRateTimer)) return;
	
	GEngine->AddOnScreenDebugMessage(10, 0.2f, FColor::Orange, "Weapon Fired");

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetParentActor());
	UCameraComponent* Camera = Player->GetFirstPersonCameraComponent();

	FVector ShotStart = Camera->GetComponentLocation();
	FVector ShotTarget = Camera->GetComponentLocation() + Camera->GetForwardVector() * 1000.0f;

	FVector DebugShotStart = MuzzleArrowComponent->GetComponentLocation();
	
	DrawDebugLineTraceSingle(GetWorld(), DebugShotStart, ShotTarget, EDrawDebugTrace::ForDuration, true, FHitResult(), FColor::Red, FColor::Purple, GetFireRateInSeconds());
	Server_Fire(ShotStart, ShotTarget, Player);

	GetWorldTimerManager().SetTimer(FireRateTimer, GetFireRateInSeconds(), false);
}

void AWeaponActor::Server_Fire_Implementation(FVector Start, FVector End, APlayerCharacter* ShotInstigator)
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldDynamic);

	if (Hit.GetActor())
	{
		Hit.GetActor()->TakeDamage(Damage, FDamageEvent(), ShotInstigator->GetController(), ShotInstigator);
	}
}

void AWeaponActor::SecondaryFire_Implementation()
{
	GEngine->AddOnScreenDebugMessage(11, 0.2f, FColor::Orange, "Weapon Fired Secondary");
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

