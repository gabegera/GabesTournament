// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterDelegates.h"
#include "ShooterEnumLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "ShooterGameState.generated.h"


UCLASS()
class GABESTOURNAMENT_API AShooterGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AShooterGameState();

protected:
	UPROPERTY(BlueprintReadOnly, Category="Scores", meta=(EditCondition = "false"))
	TMap<ETeam, int32> Scores;

public:
	UFUNCTION(BlueprintCallable)
	void AddScore(const ETeam Team, const int32 Points);

	void Tick(float DeltaSeconds) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<ETeam, int32> GetScores() const { return Scores; }
	
};
