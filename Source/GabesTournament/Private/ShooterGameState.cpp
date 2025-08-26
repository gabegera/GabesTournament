// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameState.h"

AShooterGameState::AShooterGameState()
{
	AddScore(ETeam::BlueTeam, 0);
	AddScore(ETeam::RedTeam, 0);
	AddScore(ETeam::OrangeTeam, 0);
	AddScore(ETeam::YellowTeam, 0);
	AddScore(ETeam::GreenTeam, 0);
	AddScore(ETeam::PurpleTeam, 0);
	AddScore(ETeam::CyanTeam, 0);
	AddScore(ETeam::SilverTeam, 0);
}

void AShooterGameState::AddScore(const ETeam Team, const int32 Points)
{
	Scores.Add(Team, Scores.FindRef(Team) + Points);
}

void AShooterGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
