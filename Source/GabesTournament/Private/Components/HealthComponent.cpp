// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

void UHealthComponent::TakeDamage(const float DamageAmount)
{
	Health -= DamageAmount;

	if (Health < 0) Health = 0;
}
