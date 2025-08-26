// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterEnumLibrary.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"


UCLASS()
class GABESTOURNAMENT_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	FName UserName = "Player Name";
	
	UPROPERTY(EditAnywhere)
	ETeam Team = ETeam::BlueTeam;

public:
	
	UFUNCTION(BlueprintCallable)
	ETeam SetTeam(ETeam NewTeam) { return Team = NewTeam;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ETeam GetTeam() { return Team; }
};
