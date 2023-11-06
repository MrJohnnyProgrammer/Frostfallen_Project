// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticPickUp.generated.h"

UCLASS()
class FROSTFALLEN_API AStaticPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticPickUp();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Volume")
	class USphereComponent* pickUpVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Static Mesh")
	class UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class UNiagaraSystem* AOEParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase* pickUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundAttenuation* LoudAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundConcurrency* concurrency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tyoe")
	bool bBomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tyoe")
	bool bMoon;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void pickUpOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void pickUpOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
