// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

#include "ShooterGameState.h"
#include "Pawns/Spectator.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->bUsePawnControlRotation = true;

	GetMesh()->SetupAttachment(CameraComponent);
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(true);

	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMeshComponent->SetupAttachment(CameraComponent);
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);

	WeaponChildActorComponent->SetupAttachment(FirstPersonMeshComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}



void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShowDebugInfo == true)
	{
		// FRAME RATE
		float FrameRate = FMath::RoundToInt32(1 / GetWorld()->GetDeltaSeconds()) ;
		FString FrameRateString = "FPS: " + FString::SanitizeFloat(FrameRate, 0);
		
		float MilisecondsPerFrame = 1000.0f / FrameRate;
		MilisecondsPerFrame = FMath::Floor(MilisecondsPerFrame * 100) / 100;
		FrameRateString += " | " + FString::SanitizeFloat(MilisecondsPerFrame) + " ms";
		
		GEngine->AddOnScreenDebugMessage(900, MAX_FLT, FColor::White, FrameRateString, false);

		FString String = FString();
		const TMap<ETeam, int32> Scores = GetWorld()->GetGameState<AShooterGameState>()->GetScores();

		String = "Team 1: " + Scores.FindRef(ETeam::BlueTeam);
		GEngine->AddOnScreenDebugMessage(1001, 1.0f, FColor::Blue, String);

		String = "Team 2: " + Scores.FindRef(ETeam::RedTeam);
		GEngine->AddOnScreenDebugMessage(1002, 1.0f, FColor::Red, String);

		String = "Team 3: " + Scores.FindRef(ETeam::OrangeTeam);
		GEngine->AddOnScreenDebugMessage(1003, 1.0f, FColor::Orange, String);

		String = "Team 4: " + Scores.FindRef(ETeam::YellowTeam);
		GEngine->AddOnScreenDebugMessage(1004, 1.0f, FColor::Yellow, String);
	
		String = "Team 5: " + Scores.FindRef(ETeam::GreenTeam);
		GEngine->AddOnScreenDebugMessage(1005, 1.0f, FColor::Green, String);

		String = "Team 6: " + Scores.FindRef(ETeam::PurpleTeam);
		GEngine->AddOnScreenDebugMessage(1006, 1.0f, FColor::Purple, String);

		String = "Team 7: " + Scores.FindRef(ETeam::CyanTeam);
		GEngine->AddOnScreenDebugMessage(1007, 1.0f, FColor::Cyan, String);

		String = "Team 8: " + Scores.FindRef(ETeam::SilverTeam);
		GEngine->AddOnScreenDebugMessage(1008, 1.0f, FColor::Silver, String);
	}
}

void APlayerCharacter::SetMeshColor(FColor Color)
{
	Super::SetMeshColor(Color);

	UMaterialInstanceDynamic* MaterialInstance;

	MaterialInstance = UMaterialInstanceDynamic::Create(FirstPersonMeshComponent->GetMaterial(0), this);
	MaterialInstance->SetVectorParameterValue("Tint", Color);
	FirstPersonMeshComponent->SetMaterial(0, MaterialInstance);

	MaterialInstance = UMaterialInstanceDynamic::Create(FirstPersonMeshComponent->GetMaterial(1), this);
	MaterialInstance->SetVectorParameterValue("Tint", Color);
	FirstPersonMeshComponent->SetMaterial(1, MaterialInstance);
}

void APlayerCharacter::Die(FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// ASpectator* Spectator = GetWorld()->SpawnActor<ASpectator>(ASpectator::StaticClass(), GetActorLocation() + FVector(0, 0, 10), GetActorRotation(), FActorSpawnParameters());
	//
	// Spectator->SetCharacterToSpawn(GetClass());
	// GetController()->Possess(Spectator);
	
	Super::Die(DamageEvent, EventInstigator, DamageCauser);
}



