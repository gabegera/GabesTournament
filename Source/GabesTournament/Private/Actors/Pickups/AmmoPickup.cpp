// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pickups/AmmoPickup.h"


// Sets default values
AAmmoPickup::AAmmoPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmoPickup::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (StaticMeshComponent->IsVisible() == false || GetWorldTimerManager().IsTimerActive(SpawnTimer)) return;

	if (!OtherActor->Implements<UPickupInterface>()) return;

	if (Execute_PickupAmmo(OtherActor, AmmoType, AmmoAmount))
	{
		HidePickup();
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AAmmoPickup::ShowPickup, SpawnDelay);
	}
}

