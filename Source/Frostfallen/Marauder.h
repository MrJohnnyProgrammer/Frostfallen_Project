// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Marauder.generated.h"

/**
 * 
 */
UCLASS()
class FROSTFALLEN_API AMarauder : public AEnemy
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AAxe* weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	bool dead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	bool bMagic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bBasicAttackCD;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:


	UFUNCTION(BlueprintImplementableEvent)
	void axeOnHand(bool on);

	UFUNCTION(BlueprintCallable)
	void SpawnAxe();

	UFUNCTION(BlueprintCallable)
	void RealSpawnAxe();

	UFUNCTION(BlueprintCallable)
	void JumpAttack();

	UFUNCTION(BlueprintCallable)
		void Kick();

	UFUNCTION(BlueprintCallable)
	void Scream();

	UFUNCTION(BlueprintCallable)
	void RealScream();

	UFUNCTION(BlueprintCallable)
		void SmackDown();

	UFUNCTION(BlueprintCallable)
		void RunSmack();

	UFUNCTION(BlueprintCallable)
		void Attack();

	UFUNCTION(BlueprintCallable)
		void BackAttack();

	UFUNCTION(BlueprintCallable)
		void DownAttack();

	UFUNCTION(BlueprintCallable)
		void Death();

	UFUNCTION(BlueprintCallable)
		void Stumble();		

	UFUNCTION(BlueprintCallable)
		void BodyThrowParry() ;
};
