// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceBomb.generated.h"

UCLASS()
class FROSTFALLEN_API AIceBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIceBomb();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	ACharacter* bombHolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> damageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Static Mesh")
	class UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class UNiagaraSystem* AOEParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundBase* boomSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundAttenuation* LoudAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundConcurrency* concurrency;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | hit")
	class USoundCue* explosionSound;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	virtual void OnBombHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void Thrown(const FVector& direction, FRotator throwRot, bool isAi);

	UFUNCTION()
	void EquipBomb(class ACharacter* player);

	UFUNCTION()
		void AllowCollision();



};
