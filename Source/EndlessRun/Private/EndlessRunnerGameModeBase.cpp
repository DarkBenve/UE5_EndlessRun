// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunnerGameModeBase.h"
#include "FloorTile.h"

void AEndlessRunnerGameModeBase::BeginPlay()
{
	CreateInitialFloorTiles();
}

void AEndlessRunnerGameModeBase::CreateInitialFloorTiles()
{

	AFloorTile* Tile = AddFloorTile(false);

	if (Tile)
	{
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}

	// for (float Val : LaneSwitchValues)
	// 	UE_LOG(LogTemp,Warning,TEXT("LANE VALUE: %f"),Val);
	AddFloorTile(false);
	AddFloorTile(false);
	
	for (int i=0;i<NumInitialFloorTiles;i++)
	{
		AddFloorTile(true);
	}
}

AFloorTile* AEndlessRunnerGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	UWorld* World = GetWorld();

	if (World)
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);

		if (Tile)
		{
			if (bSpawnItems)
			{
				Tile->SpawnItems();
			}
			NextSpawnPoint = Tile->GetAttachTransform();
		}

		return Tile;
	}

	return nullptr;
}