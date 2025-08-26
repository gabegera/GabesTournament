// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile.h"

#include "KismetTraceUtils.h"
#include "ShooterPlayerState.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ExplosiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ExplosiveComponent = CreateDefaultSubobject<UExplosiveComponent>(TEXT("Explosive Component"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName("NoCollision", false);
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->CanCharacterStepUpOn = ECB_No;
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetupAttachment(MeshComponent);
    CollisionSphere->SetCollisionProfileName("OverlapAllDynamic", true);
    CollisionSphere->SetGenerateOverlapEvents(true);
    CollisionSphere->CanCharacterStepUpOn = ECB_No;
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);

	bReplicates = true;
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(30.0f);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentLocation == FVector::ZeroVector)
	{
		CurrentLocation = GetActorLocation();
		PreviousLocation = GetActorLocation();
	}
	else
	{
		PreviousLocation = CurrentLocation;
		CurrentLocation = GetActorLocation();

		CheckCollision();
	}
}

void AProjectile::CheckCollision()
{
	FCollisionQueryParams IgnorePlayer;
	IgnorePlayer.AddIgnoredActor(GetUniqueID());
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PreviousLocation, CurrentLocation, ECC_WorldDynamic, IgnorePlayer);
	DrawDebugLineTraceSingle(GetWorld(), PreviousLocation, Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd, EDrawDebugTrace::ForDuration, true, FHitResult(), FColor::Red, FColor::Green, 1.0f);

	if (Hit.bBlockingHit)
	{
		SetActorLocation(Hit.ImpactPoint);
		OnOverlapBegin(CollisionSphere, Hit.GetActor(), Hit.GetComponent(), Hit.ElementIndex, false, Hit);
	}
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If other actor is a projectile then cancel collision.
	if (OtherActor->GetClass() == GetClass()) return;
	if (!IsValid(OtherActor)) return;
	
	if (DoesDealImpactDamage) Server_DealImpactDamage(OtherActor, ImpactDamage, nullptr, Spawner);

	if (DoesExplodeOnCollision) ExplosiveComponent->Explode();
	else Destroy();

	// if (DoesSpawnProjectilesOnCollision) Server_SpawnProjectiles(ProjectilesToSpawn, ProjectilesToSpawnCount, ProjectilesToSpawnDamage, SweepResult.ImpactNormal);
}

void AProjectile::Server_DealImpactDamage_Implementation(AActor* Target, float Damage, AController* DamageInstigator, AActor* DamageCauser)
{
	if (!IsValid(Target) || !Target->Implements<UDamageInterface>()) return;
	
	Execute_DealDamage(Target, Damage, FDamageEvent(), DamageInstigator, DamageCauser);
	if (!DoesExplodeOnCollision)
	{
		Execute_AddImpulseAtLocation(Target, UKismetMathLibrary::GetDirectionUnitVector(PreviousLocation, GetActorLocation()) * Damage * KnockbackForceMultiplier, GetActorLocation(), NAME_None);
	}
}

void AProjectile::Server_SpawnProjectiles_Implementation(TSubclassOf<AProjectile> Projectile, int32 NumberToSpawn, float Damage, FVector UpSpawnDirection, AController* DamageInstigator, AActor* DamageCauser)
{
	for (int i = 0; i < NumberToSpawn; i++)
	{
		AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile);

		if (SpawnedProjectile == nullptr) return;
		
		SpawnedProjectile->SetActorLocation(GetActorLocation());
		UKismetMathLibrary::RandomUnitVectorInConeInDegrees(UpSpawnDirection, 180);
		SpawnedProjectile->SetImpactDamage(Damage);
		SpawnedProjectile->SetSpawner(DamageCauser);
		SpawnedProjectile->ExplosiveComponent->SetExplosionCauser(DamageCauser);
		SpawnedProjectile->ExplosiveComponent->SetExplosionInstigator(DamageInstigator);
	}
}

