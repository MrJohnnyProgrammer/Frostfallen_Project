// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bat.generated.h"

UCLASS()
class FROSTFALLEN_API ABat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABat();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bNoisePlay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
		int32 health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh")
		class USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pick-Up Volume")
		class USphereComponent* pickUpVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Combat")
		AController* batInstigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		TSubclassOf<UDamageType> damageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Combat")
		ACharacter* batHolder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		class USoundBase* breakSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundBase* hitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
		class UBoxComponent* combatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		class USoundBase* equipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundBase* swingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundBase* flySound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		bool bThrown;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector thrownDirection;

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

	UFUNCTION()
		virtual void BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Equip(class ACharacter* player, bool isPlayer) ;

	UFUNCTION(BlueprintCallable)
		void ActivateBladeCollision();

	UFUNCTION(BlueprintCallable)
		void DeactivateBladeCollision();

	UFUNCTION()
		virtual void OnBladeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
		void Thrown(const FVector& direction, FRotator throwRot, bool isAi, FVector start);

	UFUNCTION()
		void Drop();


	UFUNCTION()
		void AllowCollision();

	FORCEINLINE  void SetWeaponInstigator(AController* instigator) { batInstigator = instigator; }

	FORCEINLINE  void SetAxeHolder(ACharacter* holder) { batHolder = holder; }

	UFUNCTION()
	void LoseHealth();
};
