// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnPoint.h"
#include "Enemy.h" 
#include "Axe.h"
#include "Bat.h"

// Sets default values
AEnemySpawnPoint::AEnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	Spawn();
}

// Called every frame
void AEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}


void AEnemySpawnPoint::Spawn()
{
	FActorSpawnParameters spawnInfo;

	spawnedEnemy = GetWorld()->SpawnActor<AEnemy>(enemyToSpawnBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);

}


void AEnemySpawnPoint::DestroySpawned()
{	
	if (spawnedEnemy)
	{
		if (spawnedEnemy->equippedAxe)
		{
			spawnedEnemy->equippedAxe->Destroy();
		}
		spawnedEnemy->Destroy();
	}
	
}