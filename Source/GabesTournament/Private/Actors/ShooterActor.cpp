// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ShooterActor.h"

#include "Components/HealthComponent.h"
#include "Components/ExplosiveComponent.h"

// Sets default values
AShooterActor::AShooterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AShooterActor::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent = GetComponentByClass<UHealthComponent>();
	ExplosiveComponent = GetComponentByClass<UExplosiveComponent>();
}

float AShooterActor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (!HealthComponent && !ExplosiveComponent) return 0.0f;

	if (HealthComponent)
	{
		HealthComponent->LoseHealth(DamageAmount);
	}

	if (ExplosiveComponent)
	{
		ExplosiveComponent->SetExplosionCauser(DamageCauser);
		ExplosiveComponent->SetExplosionInstigator(EventInstigator);
	}

	if (HealthComponent && ExplosiveComponent && HealthComponent->GetHealth() / HealthComponent->GetMaxHealth() <= 0.0f)
	{
		ExplosiveComponent->Explode();
	}
	else if (HealthComponent && ExplosiveComponent && HealthComponent->GetHealth() / HealthComponent->GetMaxHealth() <= 0.66f)
	{
		ExplosiveComponent->LightFuse();
	}
	else if (ExplosiveComponent)
	{
		ExplosiveComponent->LightFuse();
	}
	
	return DamageAmount;
}

bool AShooterActor::DealDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return true;
}

bool AShooterActor::AddRadialImpulse_Implementation(FVector ImpulseLocation, float Radius, float Force, bool Falloff)
{
	if (!StaticMeshComponent->IsSimulatingPhysics()) return false;
	
	StaticMeshComponent->AddRadialImpulse(ImpulseLocation, Radius, Force, Falloff ? RIF_Linear : RIF_Constant);
	
	return true;
}

bool AShooterActor::AddImpulseAtLocation_Implementation(FVector Impulse, FVector Location, FName BoneName)
{
	if (!StaticMeshComponent->IsSimulatingPhysics()) return false;
	
	StaticMeshComponent->AddImpulseAtLocation(Impulse, Location, BoneName);

	return true;
}

// Called every frame
void AShooterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

