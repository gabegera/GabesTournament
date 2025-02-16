// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/WeaponActor.h"

#include "KismetTraceUtils.h"
#include "Components/ArrowComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"


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
	FVector ShotTarget = Camera->GetComponentLocation();
	ShotTarget += UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Camera->GetForwardVector(), BulletSpread) * 100000.0f;

	Server_Fire(ShotStart, ShotTarget, Player);

	GetWorldTimerManager().SetTimer(FireRateTimer, GetFireRateInSeconds(), false);
}

void AWeaponActor::Server_Fire_Implementation(FVector Start, FVector End, ACharacter* Causer)
{
	FCollisionQueryParams IgnorePlayer;
	IgnorePlayer.AddIgnoredActor(Causer->GetUniqueID());
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldDynamic, IgnorePlayer);
	DrawDebugLineTraceSingle(GetWorld(), MuzzleArrowComponent->GetComponentLocation(), End, EDrawDebugTrace::ForDuration, true, FHitResult(), FColor::Red, FColor::Green, GetFireRateInSeconds());

	if (Hit.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(12, 0.2f, FColor::Purple, "Hit Actor: " + Hit.GetActor()->GetName());
		Hit.GetActor()->TakeDamage(Damage, FPointDamageEvent(), Causer->GetController(), Causer);
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

