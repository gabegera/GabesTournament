// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetRootComponent());

	GetMesh()->SetupAttachment(CameraComponent);
	GetMesh()->SetOnlyOwnerSee(true);

	ThirdPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	ThirdPersonMeshComponent->SetupAttachment(CameraComponent);
	ThirdPersonMeshComponent->SetOwnerNoSee(true);
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
	}
}
