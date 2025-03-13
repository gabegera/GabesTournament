// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterDelegates.h"
#include "GameModes/ShooterGameModeBase.h"
#include "TDMGameMode.generated.h"


UCLASS()
class GABESTOURNAMENT_API ATDMGameMode : public AShooterGameModeBase
{
	GENERATED_BODY()

public:

	ATDMGameMode();

protected:
	
	UPROPERTY(EditAnywhere)
	int32 ScoreToWin = 50;

	FCharacterHasDied CharacterHasDiedDelegate;

public:

	UFUNCTION(BlueprintCallable)
	void PlayerDied(ACharacter* DeadCharacter, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};
