// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Spectator.h"
#include "Characters/PlayerCharacter.h"

void ASpectator::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASpectator::SpawnPlayer, TimeToRespawnPlayer);
}

void ASpectator::SpawnPlayer()
{
	APlayerCharacter* SpawnedPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(CharacterToSpawn, GetActorLocation(), FRotator(0, 0, 0), FActorSpawnParameters());

	GetController()->Possess(SpawnedPlayerCharacter);

	Destroy();
	
}
