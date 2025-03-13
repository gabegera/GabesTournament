#pragma once

#include "CoreMinimal.h"
#include "ShooterDelegates.generated.h"

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCharacterHasDied, ACharacter*, DeadCharacter, struct FDamageEvent const&, DamageEvent, class AController*, EventInstigator, AActor*, DamageCauser);