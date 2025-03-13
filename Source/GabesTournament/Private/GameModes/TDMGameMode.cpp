// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TDMGameMode.h"
#include "GameFramework/Character.h"

ATDMGameMode::ATDMGameMode()
{
	CharacterHasDiedDelegate.AddUniqueDynamic(this, &ATDMGameMode::PlayerDied);
}

void ATDMGameMode::PlayerDied(ACharacter* DeadCharacter, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsValid(DeadCharacter) || !IsValid(DamageCauser)) return;
	GEngine->AddOnScreenDebugMessage(888, 5.0f, FColor::White, DeadCharacter->GetName() + " was killed by " + DamageCauser->GetName());
}
