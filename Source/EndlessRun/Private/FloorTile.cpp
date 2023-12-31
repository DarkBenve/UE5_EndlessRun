// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"

#include "CoinItem.h"
#include "EndlessRunnerGameModeBase.h"
#include "Obstacle.h"
#include "RunCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values

AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttackPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttackPoint->SetupAttachment(SceneComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Center Lane"));
	CenterLane->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Lane"));
	RightLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Lane"));
	LeftLane->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorTriggerBox"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.f, 500.f, 200.f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned

void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	check(RunGameMode);

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this,&AFloorTile::OnTriggerBoxOverlap);
}

void AFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherCamp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);

	if (RunCharacter)
	{
		RunGameMode->AddFloorTile(true);

		GetWorldTimerManager().SetTimer(DestroyHandler, this, &AFloorTile::DestroyFloorTile, 2.f, false);
	}
}

void AFloorTile::SpawnItems()
{
	if (IsValid(SmallObstacleClass) && IsValid(BigObstacleClass) && IsValid(CoinItemClass))
	{
		int32 NumBig = 0;
		SpawnLaneItem(CenterLane,NumBig);
		SpawnLaneItem(LeftLane,NumBig);
		SpawnLaneItem(RightLane,NumBig);	
	}
}

void AFloorTile::SpawnLaneItem(UArrowComponent* Lane, int32& NumBig)
{
	const float RandVal = FMath::FRandRange(0.f, 1.f);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform& SpawnLocation = Lane->GetComponentTransform();
	
	if (UKismetMathLibrary::InRange_FloatFloat(RandVal,SpawnPercent1, SpawnPercent2,true,true))
	{
		AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass,SpawnLocation,SpawnParameters);
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandVal,SpawnPercent2, SpawnPercent3,true,true))
	{
		if (NumBig <= 2)
		{
			AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(BigObstacleClass,SpawnLocation,SpawnParameters);

			if (Obstacle)
			{
				NumBig += 1;
			}	
		}
		else
		{
			AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass,SpawnLocation,SpawnParameters);
		}
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandVal,SpawnPercent3, 1.f,true,true))
	{
		ACoinItem* Coin = GetWorld()->SpawnActor<ACoinItem>(CoinItemClass,SpawnLocation,SpawnParameters);
	}
}

void AFloorTile::DestroyFloorTile()
{
	if (DestroyHandler.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandler);
	}

	this->Destroy();
}

