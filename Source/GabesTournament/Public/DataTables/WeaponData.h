#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/WeaponActor.h"
#include "WeaponData.generated.h"



USTRUCT(BlueprintType)
struct GABESTOURNAMENT_API FWeaponData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponActor> WeaponActor;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "WeaponActor != nullptr", EditConditionHides))
	// float Damage = WeaponActor.GetDefaultObject()->Damage;
};