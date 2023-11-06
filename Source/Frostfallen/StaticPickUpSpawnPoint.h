// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticPickUpSpawnPoint.generated.h"

UCLASS()
class FROSTFALLEN_API AStaticPickUpSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticPickUpSpawnPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class TSubclassOf<AActor> bombToSpawnBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class TSubclassOf<AActor> moonToSpawnBlueprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class TSubclassOf<AActor> axeToSpawnBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class TSubclassOf<AActor> batToSpawnBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class TSubclassOf<AActor> hitableDoorToSpawnBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class TSubclassOf<AActor> shearingDoorToSpawnBlueprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bHitableDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class AhitableDoor* hitableDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bShearingDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class AShearableDoor* shearingDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bAxe;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class AAxe* spawnedAxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bBat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class ABat* spawnedBat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIceBombs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class AStaticPickUp* spawnedIceBombOrMoon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bMoons;


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
