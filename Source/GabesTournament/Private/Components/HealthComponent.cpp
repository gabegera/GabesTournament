// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);

	Health = MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
}

float UHealthComponent::LoseHealth(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health < 0) Health = 0;
	
	//Server_TakeDamage(DamageAmount);
	
	return Health;
}

void UHealthComponent::Server_TakeDamage_Implementation(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health < 0) Health = 0;
}

void UHealthComponent::Multicast_TakeDamage_Implementation(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health < 0) Health = 0;	
}

