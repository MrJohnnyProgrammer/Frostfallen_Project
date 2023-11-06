// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Marauder.h"
#include "Thrower.h"
#include "MagicMarauder.generated.h"

/**
 * 
 */
UCLASS()
class FROSTFALLEN_API AMagicMarauder : public AMarauder
{
	GENERATED_BODY()

public:
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Throw();

	UFUNCTION(BlueprintCallable)
	void RealThrow();

	UFUNCTION(BlueprintCallable)
		void Taunt();
	
};
