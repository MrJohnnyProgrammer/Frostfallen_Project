// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Bomber.generated.h"

/**
 * 
 */
UCLASS()
class FROSTFALLEN_API ABomber : public AEnemy
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		class AIceBomb* equippedBomb;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<AActor> bombBlueprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		FVector throwingLoc;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "bomb")

	



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void RealSpawnBomb();

	UFUNCTION(BlueprintCallable)
	void Throw(FVector throwLocation);

	UFUNCTION(BlueprintCallable)
	void RealThrow();

	UFUNCTION(BlueprintCallable)
		void Death();


	UFUNCTION(BlueprintCallable)
		void Stumble();

};
