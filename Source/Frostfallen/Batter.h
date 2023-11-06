// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Marauder.h"
#include "Batter.generated.h"

/**
 * 
 */
UCLASS()
class FROSTFALLEN_API ABatter : public AMarauder
{
	GENERATED_BODY()
public:

//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
		//class AAxe* weapon; Bat


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
};
