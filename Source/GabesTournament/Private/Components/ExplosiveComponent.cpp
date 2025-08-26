// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ExplosiveComponent.h"

#include "ShooterPlayerState.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UExplosiveComponent::UExplosiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}

// Called when the game starts
void UExplosiveComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UExplosiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UExplosiveComponent::LightFuse()
{
	if (FuseTime > 0)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(FuseTimerHandle)) return;
		GetWorld()->GetTimerManager().SetTimer(FuseTimerHandle, this, &UExplosiveComponent::Explode, FuseTime, false);
	}
	else Explode();
}

void UExplosiveComponent::Explode()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Server_Explode();
	GetWorld()->GetTimerManager().SetTimer(DestructionTimerHandle, this, &UExplosiveComponent::DestroyOwner, 10.0f, false);
}

void UExplosiveComponent::Server_Explode_Implementation()
{
	TArray<FHitResult> Hits;
	// TArray<AActor*> ActorsToIgnore;
	// ActorsToIgnore.Add(GetOwner());
	// TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	//
	// UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(), ExplosionRadius, ObjectTypes,
	// 	false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hits, true,
	// 	FLinearColor::Red, FLinearColor::Green, 0.4f);

	GetWorld()->SweepMultiByChannel(Hits, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(), GetOwner()->GetActorQuat(),
		ECC_WorldDynamic, FCollisionShape::MakeSphere(ExplosionRadius));

	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), GetExplosionRadius(), 32, FColor::Orange, false, 0.5f);

	DisableOwner();
	Multicast_Explode();
	
	for (FHitResult& Hit : Hits)
	{
		if (!IsValid(Hit.GetActor()) || !Hit.GetActor()->Implements<UDamageInterface>()) continue;
		if (Hit.GetActor() == GetOwner()) continue;
		Execute_DealDamage(Hit.GetActor(), ExplosionDamage, FDamageEvent(), ExplosionInstigator, ExplosionCauser);
		Execute_AddRadialImpulse(Hit.GetActor(), GetOwner()->GetActorLocation(), ExplosionRadius * 2, ExplosionDamage * ExplosionForceMultiplier, true);
	}
	
}

void UExplosiveComponent::Multicast_Explode_Implementation()
{
	DisableOwner();
}

void UExplosiveComponent::DisableOwner()
{
	GetOwner()->SetActorHiddenInGame(true);
	GetOwner()->SetActorEnableCollision(false);
	GetOwner()->SetActorTickEnabled(false);
}

void UExplosiveComponent::DestroyOwner()
{
	Server_DestroyOwner();
}

void UExplosiveComponent::Server_DestroyOwner_Implementation()
{
	GetOwner()->Destroy();
}
