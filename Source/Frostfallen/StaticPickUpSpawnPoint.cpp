// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticPickUpSpawnPoint.h"
#include "StaticPickUp.h"
#include "Axe.h"
#include "Bat.h"
#include "hitableDoor.h"
#include "ShearableDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Templates/SubclassOf.h"

// Sets default values
AStaticPickUpSpawnPoint::AStaticPickUpSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAxe = false;
	bBat = false;
	bIceBombs = false;
	bMoons = false;
	bHitableDoor = false;
	bShearingDoor = false;
}

// Called when the game starts or when spawned
void AStaticPickUpSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	Spawn();
}

// Called every frame
void AStaticPickUpSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void AStaticPickUpSpawnPoint::Spawn()
{
	FActorSpawnParameters spawnInfo;

	if (bAxe && axeToSpawnBlueprint)
	{
		spawnedAxe = GetWorld()->SpawnActor<AAxe>(axeToSpawnBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
	}
	else if (bBat && batToSpawnBlueprint)
	{
		spawnedBat = GetWorld()->SpawnActor<ABat>(batToSpawnBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
	}
	else if (bMoons && bombToSpawnBlueprint)
	{
		
		spawnedIceBombOrMoon = GetWorld()->SpawnActor<AStaticPickUp>(moonToSpawnBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);

	}
	else if (bIceBombs && moonToSpawnBlueprint)
	{
		spawnedIceBombOrMoon = GetWorld()->SpawnActor<AStaticPickUp>(bombToSpawnBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);

	}
	else if (bHitableDoor && hitableDoorToSpawnBlueprint)
	{
		hitableDoor = GetWorld()->SpawnActor<AhitableDoor>(hitableDoorToSpawnBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
	}
	else if (bShearingDoor && shearingDoorToSpawnBlueprint)
	{
		shearingDoor = GetWorld()->SpawnActor<AShearableDoor>(shearingDoorToSpawnBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
	}
}

void AStaticPickUpSpawnPoint::DestroySpawned()
{
	if (bAxe)
	{
		if(spawnedAxe)
			spawnedAxe->Destroy();
	}
	else if (bBat)
	{
		if (spawnedBat)
		spawnedBat->Destroy();
	}	
	else if(bIceBombs || bMoons)
	{
		if (spawnedIceBombOrMoon)
		spawnedIceBombOrMoon->Destroy();
	}
	else if (bHitableDoor && hitableDoor)
	{
		hitableDoor->Destroy();
	}
	else if (bShearingDoor && shearingDoor)
	{
		shearingDoor->Destroy();
	}


	Spawn();
}

