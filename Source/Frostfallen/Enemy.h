// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"



UENUM(BlueprintType)
enum class EEnemyMovementStatus	 : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_Walk UMETA(DisplayName = "Walk"),
	EMS_Run UMETA(DisplayName = "Run"),
	EMS_Sprint UMETA(DisplayName = "Sprint"),
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAX UMETA(DisplayName = "MAX")
};

UCLASS()
class FROSTFALLEN_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterialInterface* frozenMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterialInterface* regularMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh")
		class USkeletalMeshComponent* headSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh")
		class USkeletalMeshComponent* clothingSkeletalMesh;

	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* torso;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* rUpArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* rDownArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* lDownArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* lUpArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* rDownLeg;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* rUpLeg;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* lDownLeg;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	UCapsuleComponent* lUpLeg;
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
	class UBoxComponent* kickBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	class UBoxComponent* combatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	EEnemyMovementStatus enemyMovementStatus;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
		float neckRotYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
		float neckRotPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bFrozen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bAllowIPRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<UDamageType> damageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<AActor> weaponBlueprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bAiMoving;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline")
	class UTimelineComponent* rotationTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UCurveFloat* fCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bStrafing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bThrower;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bBomber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	bool bBatter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	bool bMarauder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bPickedStrafeDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bLeftRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bBackForth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bLeftRight2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AActor* locker;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bStumbling;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bear;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		AController* kickInstigator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class	AController* combatInstigator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Axe")
	class AAxe* equippedAxe;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Axe")
		class ABat* equippedBat;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Axe")
	bool bAxeEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FVector spawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<FVector> navPathPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 navPathIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector endPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector playerToAi;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool canSeePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool bPlayerDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class ADefaultUnreal5Character* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* attackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual | Animation")
		UAnimInstance* animInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | step | snow")
		class UMaterialInterface* snowStepDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | step | inside")
		class UMaterialInterface* insideStepDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | step | inside")
		class USoundCue* insideStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | step | inside")
		class UNiagaraSystem* insideStepParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | hit")
		class UNiagaraSystem* hitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundBase* hitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundBase* kickedSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundBase* parryingSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundBase* parrySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* swingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundBase* kickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundBase* screamSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundAttenuation* LoudAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundAttenuation* quietAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundConcurrency* concurrency;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class UInitialActiveSoundParams* soundParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | step | snow")
		class USoundCue* snowStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | step | snow")
		class UNiagaraSystem* snowStepParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | step | snow")
	class UNiagaraSystem* screamVFX;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION()
		void Unfreeze();

public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetUpEnemyMovementStatus(EEnemyMovementStatus status) { enemyMovementStatus = status; }

	FORCEINLINE void SetEquippedAxe(class AAxe* axe) { equippedAxe = axe; }

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
	void DirectionMovement(TArray<FVector> pathPoints);

	UFUNCTION(BlueprintCallable)
	void Strafe();

	UFUNCTION(BlueprintCallable)
		void RotateTowardsPlayer();

	UFUNCTION(BlueprintCallable)
		void NeckRotator();

	UFUNCTION(BlueprintImplementableEvent)
	void Parried();

	UFUNCTION(BlueprintImplementableEvent)
	void Kicked();

	UFUNCTION(BlueprintImplementableEvent)
		void Killed();


	UFUNCTION(BlueprintImplementableEvent)
	void BodyParry();

	UFUNCTION(BlueprintImplementableEvent)
	void BatHitBP();

	UFUNCTION(BlueprintCallable)
	void BatHitAnimation();

	UFUNCTION(BlueprintCallable)
		void PlayerPullAxe();

	UFUNCTION(BlueprintCallable)
		void SetupKickCollision(bool activate);

	UFUNCTION()
		virtual void BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void RotationTimeLineFloatReturn(float value); 

};
