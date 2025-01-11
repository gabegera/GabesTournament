// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTables/WeaponData.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AWeaponActor;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GABESTOURNAMENT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditDefaultsOnly, Category="Inventory", meta=(RowType="WeaponData"))
	TObjectPtr<const UDataTable> WeaponsDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("Content/DataTables/DT_Weapons")));

	UPROPERTY(EditDefaultsOnly, Category="Inventory", meta=(ToolTip="Click to Refresh list of Starting Weapons. Use whenever changes are made to the Weapons Data Table."))
	bool RefreshWeaponsDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TMap<FName, bool> StartingWeapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory", meta=(EditCondition=false))
	TArray<FWeaponData> OwnedWeapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory", meta=(EditCondition=false))
	TMap<EAmmoType, int> AmmoMap;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	TArray<FWeaponData>& GetOwnedWeapons() { return OwnedWeapons; }

	UFUNCTION(BlueprintCallable)
	void RefreshOwnedWeapons();
	
	void AddWeapon(const FWeaponData& NewWeapon);

	void AddAmmo(EAmmoType AmmoType, int32 AmmoAmount);
	
};