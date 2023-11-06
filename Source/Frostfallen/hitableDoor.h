// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "hitableDoor.generated.h"

UCLASS()
class FROSTFALLEN_API AhitableDoor : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AhitableDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<AActor> weaponBlueprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Static Mesh")
	class UStaticMeshComponent* staticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh")
		class USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class AAxe* doorAxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bAxeOnDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bSpawnShearingAxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bShearable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bFrozen;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundBase* doorBreakSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundAttenuation* LoudAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundAttenuation* quietAtten;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundConcurrency* concurrency;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		TSubclassOf<UDamageType> damageTypeClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyFR();
};
