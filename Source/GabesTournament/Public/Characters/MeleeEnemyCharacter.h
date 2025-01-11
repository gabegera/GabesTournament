// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "MeleeEnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EMeleeEnemyType : uint8
{
	Grunt = 0 UMETA(DisplayName = "Grunt"),
	Tank = 1 UMETA(DisplayName = "Tank"),
};


UCLASS()
class GABESTOURNAMENT_API AMeleeEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EMeleeEnemyType GetEnemyType() const { return EnemyType; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI Settings")
	EMeleeEnemyType EnemyType = EMeleeEnemyType::Grunt;
	
};
