// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "NPC.generated.h"

/**
 * 
 */
UCLASS()
class FROSTFALLEN_API ANPC : public AEnemy
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<USoundBase*> dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<USoundBase*> endDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	USoundBase* CurrentDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FText> subs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int textIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FString> npcSubs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	class USphereComponent* interactionRange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

};
