// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCS.generated.h"

UCLASS()
class FROSTFALLEN_API ANPCS : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCS();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bPlayerInRange;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<FVector> navPathPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 navPathIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector endPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	float neckRotYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	float neckRotPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class ADefaultUnreal5Character* player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	void DialogueTrigger(bool next, ADefaultUnreal5Character* getPlayer);

	void EndDialogueShort();

	UFUNCTION()
		void DialogueOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DialogueOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void DirectionMovement(TArray<FVector> pathPoints);

	UFUNCTION()
		void NeckRotator();

};
