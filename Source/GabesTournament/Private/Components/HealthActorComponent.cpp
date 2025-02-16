// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthActorComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthActorComponent::UHealthActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);

	Health = MaxHealth;
}


// Called when the game starts
void UHealthActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthActorComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthActorComponent, Health);
}

float UHealthActorComponent::TakeDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health < 0) Health = 0;
	
	//Server_TakeDamage(DamageAmount);
	
	return Health;
}

void UHealthActorComponent::Server_TakeDamage_Implementation(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health < 0) Health = 0;
}

void UHealthActorComponent::Multicast_TakeDamage_Implementation(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health < 0) Health = 0;	
}

