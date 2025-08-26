// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "Spectator.generated.h"

class APlayerCharacter;

UCLASS()
class GABESTOURNAMENT_API ASpectator : public ASpectatorPawn
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TSubclassOf<APlayerCharacter> CharacterToSpawn;

	UPROPERTY(EditAnywhere)
	float TimeToRespawnPlayer = 5.0f;

	UPROPERTY(EditAnywhere)
	FTimerHandle RespawnTimer;

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void SpawnPlayer();

	UFUNCTION(BlueprintCallable)
	void SetCharacterToSpawn(const TSubclassOf<APlayerCharacter> Character) { CharacterToSpawn = Character; }
};
