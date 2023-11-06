// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Thrower.generated.h"

/**
 * 
 */
UCLASS()
class FROSTFALLEN_API AThrower : public AEnemy
{
	GENERATED_BODY()
	
public:



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	class UBoxComponent* headBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	AAxe* headAxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	bool bAxeOnHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	bool dead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		FVector throwingLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<AActor> AOESwordDown;

protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		// Called every frame
		virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Stumble();

	UFUNCTION(BlueprintCallable)
		void Throw(FVector throwLocation);

	UFUNCTION(BlueprintCallable)
		void RealThrow();

	UFUNCTION(BlueprintCallable)
		void Kick();

	UFUNCTION(BlueprintCallable)
		void Death();

	UFUNCTION(BlueprintCallable)
		void PullAxe();


	UFUNCTION(BlueprintCallable)
		void SpawnAxe();

	UFUNCTION(BlueprintCallable)
		void RealSpawnAxe();

	UFUNCTION(BlueprintCallable)
		void BodyThrowParry();

	UFUNCTION(BlueprintImplementableEvent)
		void axeOnHand(bool on);
};
