// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

class ACoinItem;
class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;
class AObstacle;

UCLASS()
class ENDLESSRUN_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Config")
	TSubclassOf<AObstacle> SmallObstacleClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Config")
	TSubclassOf<AObstacle> BigObstacleClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Config")
	TSubclassOf<ACoinItem> CoinItemClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* AttackPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* CenterLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* RightLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* LeftLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* FloorTriggerBox;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Config")
	float SpawnPercent1 = 0.1f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Config")
	float SpawnPercent2 = 0.3f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Config")
	float SpawnPercent3 = 0.5f;

	UFUNCTION(BlueprintCallable)
	void SpawnItems();

	
	// Sets default values for this actor's properties
	AFloorTile();

	FORCEINLINE const FTransform& GetAttachTransform() const
	{
		return AttackPoint->GetComponentTransform(); 
	}

protected:
	UPROPERTY(VisibleInstanceOnly)
	class AEndlessRunnerGameModeBase* RunGameMode;

	UPROPERTY()
	FTimerHandle DestroyHandler;

	UFUNCTION()
	void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherCamp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void SpawnLaneItem(UArrowComponent* Lane, int32& NumBig);

	UFUNCTION()
	void DestroyFloorTile();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

};
