// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PickupActor.h"
#include "Characters/ShooterPlayerCharacter.h"
#include "DataTables/AmmoData.h"

// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PickupStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Static Mesh"));
	PickupStaticMesh->SetupAttachment(RootComponent);

	PickupSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pickup Skeletal Mesh"));
	PickupSkeletalMesh->SetupAttachment(RootComponent);
	
	InteractSphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Weapon Trigger"));
	InteractSphereTrigger->SetupAttachment(RootComponent);
	InteractSphereTrigger->SetSphereRadius(120.0f);
	InteractSphereTrigger->SetAutoActivate(true);
	
	PickupSphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Trigger"));
	PickupSphereTrigger->SetupAttachment(RootComponent);
	PickupSphereTrigger->SetSphereRadius(40.0f);
	PickupSphereTrigger->SetAutoActivate(true);
	
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	PickupSphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::BeginPickupSphereOverlap);
}

void APickupActor::BeginPickupSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// switch (PickupType)
	// {
	// case EPickupType::Ammo:
	// 	if (Execute_PickupAmmo(OtherActor, AmmoData.AmmoType, AmmoData.PickupAmount))
	// 	{
	// 		Destroy();
	// 	}
	// 	break;
	// case EPickupType::Consumable:
	// 	if (Execute_PickupConsumable(OtherActor, ConsumableData.ConsumableEffect, ConsumableData.PickupAmount))
	// 	{
	// 		Destroy();
	// 	}
	// 	break;
	// case EPickupType::Equipment:
	// 	if (Execute_PickupEquipment(OtherActor, EquipmentData))
	// 	{
	// 		Destroy();
	// 	}
	// 	break;
	// case EPickupType::Weapon:
	// 	if (Execute_PickupWeapon(OtherActor, WeaponData))
	// 	{
	// 		Destroy();
	// 	}
	// 	break;	
	// }
}

void APickupActor::OnConstruction(const FTransform& Transform)
{	
	RefreshPickup();
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.0f, 90.0f * DeltaTime, 0.0f));
}

void APickupActor::SetOutlineColor(FLinearColor Color)
{
	if (!IsValid(OutlineMaterialInstance)) return;

	UMaterialInstanceDynamic* DynamicOutline = UMaterialInstanceDynamic::Create(OutlineMaterialInstance, this);
	
	DynamicOutline->SetVectorParameterValue(FName("Color"), Color);
	PickupStaticMesh->SetOverlayMaterial(DynamicOutline);
}

void APickupActor::RefreshPickup()
{
	// UStaticMesh* PickupMesh = nullptr;
	// FColor PickupOutlineColor = FColor::Black;
	//
	// switch (PickupType)
	// {
	// case EPickupType::Ammo:
	// 	if (AmmoDataTableRowHandle.IsNull() || AmmoDataTableRowHandle.RowName == NAME_None) return;
	// 	if (GetAmmoPickupMesh()) PickupMesh = GetAmmoPickupMesh();
	// 	PickupOutlineColor = GetAmmoOutlineColor();
	// 	break;
	// case EPickupType::Consumable:
	// 	if (ConsumableDataTableRowHandle.IsNull() || ConsumableDataTableRowHandle.RowName == NAME_None) return;
	// 	if (GetConsumableMesh()) PickupMesh = GetConsumableMesh();
	// 	PickupOutlineColor = GetConsumableOutlineColor();
	// 	break;
	// case EPickupType::Equipment:
	// 	if (EquipmentDataTableRowHandle.IsNull() || EquipmentDataTableRowHandle.RowName == NAME_None) return;
	// 	if (GetEquipmentMesh()) PickupMesh = GetEquipmentMesh();
	// 	PickupOutlineColor = GetEquipmentOutlineColor();
	// 	break;
	// case EPickupType::Weapon:
	// 	if (WeaponDataTableRowHandle.IsNull() || WeaponDataTableRowHandle.RowName == NAME_None) return;
	// 	if (GetWeaponMesh()) PickupMesh = GetWeaponMesh();
	// 	PickupOutlineColor = GetWeaponOutlineColor();
	// 	break;
	// }
	//
	// SetOutlineColor(PickupOutlineColor);
	// if (PickupMesh)
	// {
	// 	PickupStaticMesh->SetStaticMesh(PickupMesh);
	// 	PickupStaticMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	// }
}

// FDataTableRowHandle APickupObject::SetWeaponPickup(FDataTableRowHandle NewWeapon)
// {
// 	WeaponPickup = NewWeapon;
// 	RefreshPickup();
// 	return WeaponPickup;
// }
//
// FDataTableRowHandle APickupObject::SetItemPickup(FDataTableRowHandle NewItem)
// {
// 	ItemPickup = NewItem;
// 	RefreshPickup();
// 	return ItemPickup;
// }

