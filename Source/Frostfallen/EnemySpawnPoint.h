// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnPoint.generated.h"

UCLASS()
class FROSTFALLEN_API AEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnPoint();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "enemy")
	class AEnemy* spawnedEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "enemy")
	TSubclassOf<AActor> enemyToSpawnBlueprint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Spawn();

	UFUNCTION()
	void DestroySpawned();

};
