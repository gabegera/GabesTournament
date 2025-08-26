// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShooterCharacter.h"

#include "Components/CapsuleComponent.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	
	WeaponChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	WeaponChildActorComponent->SetupAttachment(GetMesh());
	WeaponChildActorComponent->SetHiddenInGame(true);

	ThirdPersonWeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonWeapon"));
	ThirdPersonWeaponMeshComponent->SetupAttachment(GetMesh());
	ThirdPersonWeaponMeshComponent->SetOwnerNoSee(true);

	bReplicates = true;

	CharacterHasDiedDelegate.AddUniqueDynamic(this, &ThisClass::AddDeathToKillfeed);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryComponent->GetWeapons().IsEmpty() == false)
	{
		WeaponChildActorComponent->SetChildActorClass(InventoryComponent->GetWeapons().begin()->Get());
		if (IsValid(GetWeaponChildActorComponent()->GetChildActor()->GetComponentByClass<USkeletalMeshComponent>()))
		{
			ThirdPersonWeaponMeshComponent->SetSkeletalMesh(GetWeaponChildActorComponent()->GetChildActor()->GetComponentByClass<USkeletalMeshComponent>()->GetSkeletalMeshAsset());
		}
	}

	Server_OwnerOnlySeeWeapon(OwnerOnlySeeWeapon);

	if (!GetController() || !GetController()->GetPlayerState<AShooterPlayerState>()) return;
	switch (GetController()->GetPlayerState<AShooterPlayerState>()->GetTeam())
	{
	case ETeam::BlueTeam:
		SetMeshColor(FColor::Blue);
		break;
	case ETeam::RedTeam:
		SetMeshColor(FColor::Red);
		break;
	case ETeam::OrangeTeam:
		SetMeshColor(FColor::Orange);
		break;
	case ETeam::YellowTeam:
		SetMeshColor(FColor::Yellow);
		break;
	case ETeam::GreenTeam:
		SetMeshColor(FColor::Green);
		break;
	case ETeam::PurpleTeam:
		SetMeshColor(FColor::Purple);
		break;
	case ETeam::CyanTeam:
		SetMeshColor(FColor::Cyan);
		break;
	case ETeam::SilverTeam:
		SetMeshColor(FColor::Silver);
		break;
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	HealthComponent->LoseHealth(DamageAmount);
	if (HealthComponent->GetHealth() <= 0) Die(DamageEvent, EventInstigator, DamageCauser);
	
	return DamageAmount;
}

bool AShooterCharacter::DealDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return true;
}

bool AShooterCharacter::AddRadialImpulse_Implementation(FVector ImpulseLocation, float Radius, float Force, bool Falloff)
{
	if (!GetMesh()->IsSimulatingPhysics()) return false;

	GetMesh()->AddRadialImpulse(ImpulseLocation, Radius, Force, Falloff ? RIF_Linear : RIF_Constant);
	
	return true;
}

bool AShooterCharacter::AddImpulseAtLocation_Implementation(FVector Impulse, FVector Location, FName BoneName)
{
	if (!GetMesh()->IsSimulatingPhysics()) return false;

	GetMesh()->AddImpulseAtLocation(Impulse, Location, BoneName);
	
	return true;
}


// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterCharacter::SetMeshColor(FColor Color)
{
	UMaterialInstanceDynamic* MaterialInstance;

	MaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	MaterialInstance->SetVectorParameterValue("Tint", Color);
	GetMesh()->SetMaterial(0, MaterialInstance);

	MaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);
	MaterialInstance->SetVectorParameterValue("Tint", Color);
	GetMesh()->SetMaterial(1, MaterialInstance);
	
}

// bool AShooterCharacter::PickupWeapon_Implementation(TSubclassOf<AWeapon> Weapon)
// {
// 	return InventoryComponent->AddWeapon(Weapon);
// }

bool AShooterCharacter::PickupAmmo_Implementation(EAmmoType AmmoType, int32 AmmoAmount)
{
	return InventoryComponent->AddAmmo(AmmoType, AmmoAmount);
}

void AShooterCharacter::Server_OwnerOnlySeeWeapon_Implementation(bool isTrue)
{
	Multicast_SetIsOtherWeaponsHidden(isTrue);
	Client_SetIsOwnerWeaponHidden(!isTrue);
}

void AShooterCharacter::Multicast_SetIsOtherWeaponsHidden_Implementation(bool isHidden)
{
	WeaponChildActorComponent->SetHiddenInGame(isHidden);
}

void AShooterCharacter::Client_SetIsOwnerWeaponHidden_Implementation(bool isHidden)
{
	WeaponChildActorComponent->SetHiddenInGame(isHidden);
}

void AShooterCharacter::Die(FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ActivateRagdoll();

	CharacterHasDiedDelegate.Broadcast(this, DamageEvent, EventInstigator, DamageCauser);

	StartDespawnTimer(TimeToDespawnAfterDeath);
}

void AShooterCharacter::ActivateRagdoll()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ThirdPersonWeaponMeshComponent->SetHiddenInGame(true);
	WeaponChildActorComponent->SetHiddenInGame(true);
}

void AShooterCharacter::DeactivateRagdoll()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	ThirdPersonWeaponMeshComponent->SetHiddenInGame(false);
	WeaponChildActorComponent->SetHiddenInGame(false);
}

void AShooterCharacter::StartDespawnTimer(float Seconds)
{
	FTimerHandle DespawnTimer;

	if (Seconds <= 0) Despawn();
	else
	{
		GetWorldTimerManager().SetTimer(DespawnTimer, this, &AShooterCharacter::Despawn, Seconds);
	}
}

void AShooterCharacter::Despawn()
{
	Destroy();
}

void AShooterCharacter::AddDeathToKillfeed(ACharacter* DeadCharacter, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsValid(DeadCharacter) || !IsValid(DamageCauser)) return;
	GEngine->AddOnScreenDebugMessage(888, 5.0f, FColor::White, DeadCharacter->GetName() + " was killed by " + DamageCauser->GetName());
}

// ------ GETTERS ------



AWeapon* AShooterCharacter::GetEquippedWeapon()
{
	if (IsValid(EquippedWeapon) && EquippedWeapon.GetClass() == GetWeaponChildActorComponent()->GetChildActor()->GetClass()) return EquippedWeapon;

	return SetEquippedWeapon(Cast<AWeapon>(GetWeaponChildActorComponent()->GetChildActor()));
}

