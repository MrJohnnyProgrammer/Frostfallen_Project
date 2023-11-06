// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Axe.generated.h"

UCLASS()
class FROSTFALLEN_API AAxe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAxe();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bNoisePlay;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bThrown;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bOnHead;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bDoor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool player;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bearAss;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bspawned;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bPermaSpawned;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 health;
		
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh")
		class USkeletalMeshComponent* skeletalMesh;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pick-Up Volume")
		class USphereComponent* pickUpVolume;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Combat")
		AController* axeInstigator;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		TSubclassOf<UDamageType> damageTypeClass;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Combat")
		APawn* axeHolder;
		
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Combat")
		AEnemy* axeHeadHolder;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Combat")
		class AhitableDoor* axeDoorHolder;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
		class UBoxComponent* combatCollision;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
			class UBoxComponent* pullCollision;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
			class UBoxComponent* headCollision;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
			class USoundBase* equipSound;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
			class USoundBase* breakSound;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
			class USoundBase* breakIceSound;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
			class USoundBase* hitThrowSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
			class USoundBase* swingSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
			class USoundBase* hitSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
			class USoundBase* flySound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
			class UAudioComponent* flyingSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
			UMaterialInterface* frozenMaterial;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		class UNiagaraSystem* sparksVFX;


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
			class USoundAttenuation* LoudAttenuation;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
			class USoundAttenuation* quietAtten;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
			class USoundConcurrency* concurrency;

		UPROPERTY(VisibleAnywhere, Category = "Movement")
			class UProjectileMovementComponent* ProjectileMovementComponent;

		UPROPERTY(VisibleAnywhere, Category = "Movement")
			class URotatingMovementComponent* RotatingMovementComponent; 

		UPROPERTY(VisibleAnywhere, Category = "Movement")
		FRotator playerRot;

		UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector thrownDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	virtual void pickUpOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;

	UFUNCTION()
	virtual void pickUpOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;

	UFUNCTION()
		virtual void BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void BladeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void pullCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void pullCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE  void SetWeaponInstigator(AController* instigator) { axeInstigator = instigator; }

	FORCEINLINE  void SetAxeHolder(APawn* holder) { axeHolder = holder; }
	
	UFUNCTION()
	void Equip(ACharacter* playableCharacter, bool isPlayer);

	UFUNCTION(BlueprintCallable)
	void ActivateBladeCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateBladeCollision();

	UFUNCTION()
	virtual void OnBladeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable)
	void Thrown(const FVector& direction, FRotator throwRot, bool isAi, FVector start);

	UFUNCTION(BlueprintCallable)
	void HeadAxeAttach(AEnemy* enemy, FName socket);

	UFUNCTION(BlueprintCallable)
	void PullAxeFromHead(AController* instigrator);
	
	UFUNCTION(BlueprintCallable)
	void PullAxeAnimation();

	UFUNCTION(BlueprintCallable)
	void Drop();

	UFUNCTION()
	void DeSpawn();

	UFUNCTION()
	void AllowCollision();

	UFUNCTION()
	void DoorAxeAttach(AhitableDoor* door);

	UFUNCTION()
	void LoseHealth();
};
