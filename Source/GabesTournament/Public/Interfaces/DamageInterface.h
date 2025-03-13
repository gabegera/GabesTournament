// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

class GABESTOURNAMENT_API IDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool DealDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AddRadialImpulse(FVector ImpulseLocation, float Radius, float Force, bool Falloff = true);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AddImpulseAtLocation(FVector Impulse, FVector Location, FName BoneName = NAME_None);
};
