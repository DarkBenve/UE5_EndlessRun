// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameModeBase.generated.h"

class AFloorTile;

UCLASS()
class ENDLESSRUN_API AEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Category="Config")
	TSubclassOf<AFloorTile> FloorTileClass;

	UPROPERTY(VisibleAnywhere)
	int32 TotalCoins = 0;
	
	UPROPERTY(EditAnywhere,Category="Config")
	int32 NumInitialFloorTiles = 10;

	UPROPERTY(VisibleInstanceOnly,Category="Runtime")
	FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly,Category="Runtime")
	TArray<float> LaneSwitchValues;
	
	UFUNCTION(BlueprintCallable)
	void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable)
	AFloorTile* AddFloorTile(const bool bSpawnItems);

	UFUNCTION()
	void AddCoin();

protected:
	virtual void BeginPlay() override;
};
