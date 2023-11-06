// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "hitableDoor.h"
#include "ShearableDoor.generated.h"

/**
 * 
 */
UCLASS()
class FROSTFALLEN_API AShearableDoor : public AhitableDoor
{
	GENERATED_BODY()
protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
};
