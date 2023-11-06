	// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DefaultUnreal5Character.generated.h"


UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_Walk UMETA(DisplayName = "Walk"),
	EMS_Run UMETA(DisplayName = "Run"),
	EMS_Sprint UMETA(DisplayName = "Sprint"),
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAX UMETA(DisplayName = "MAX")
};


UCLASS(config=Game)
class ADefaultUnreal5Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;


public:
	ADefaultUnreal5Character();

	

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */

	//bools
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		bool bMainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		bool bFallDeath;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		bool bReadyToDisplayPickUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		bool bPause;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	bool bResurrecting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	bool bMovingForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	bool bMovingRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bDodge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bDodgeBufferWindow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bDodgeBuffer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bDodgeBufferExecute;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bNextDodge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bDoubleSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bOverlappingPickup;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bSprintCameraRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bSprintCamera1st;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool bToggledOn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
		bool headAxe;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bFrozen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bBeforeFreeze;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bFreezing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bNextAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bStumble;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bBatHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bNotDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bCanMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item | Combat")
		bool bequippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "item | Combat")
		bool bStringOpen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "item | Combat")
		bool bPickUpBomb;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item | Combat")
		int health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item | Combat")
		int bombs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item")
		int moons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item | Combat")
		int maxBombs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item | Combat")
		bool binAxeRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "item | Combat")
		bool bPullingAxe;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
		bool binNPCRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
		bool bInDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
		bool bStopDialogue;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		bool cameraBack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	bool reverseSubs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	bool reverseDeathScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool reverseEndScreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	bool AlphaEnding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bReversePickedUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	bool bPickedUpAxe;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	bool bPickedUpBat;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	bool bPickedUpBomb;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	bool bPickedUpMoon;


	//Blueprint subclasses
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<AActor> bombBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<AActor> axeBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
		TSubclassOf<AActor> batBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<AActor> enemySpawnBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<AActor> pickUpSpawnBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Input)
	float TurnRateGamepad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float TurnRateGamepadAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float currentLowerArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float yawTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float pitchTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class	AController* combatInstigator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	AActor* toggledOnActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	class AEnemy* enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	class ANPCS* npc;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* characterAnims;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* characterAnimsUpper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimInstance* animInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toggle")
	TArray<AActor*> inRangeEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toggle")
	AActor* closestAIToTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Combat")
	TSubclassOf<UDamageType> damageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Axe")
	class AAxe* equippedAxe;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item | Combat")
	class AIceBomb* equippedBomb;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ENum")
	EMovementStatus movementStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "item | Combat")
	class AAxe* activeOverlappedAxe;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item ")
	class AStaticPickUp* activeOverlappedStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "spawn")
		FVector spawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subs")
		FText subText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item | Combat")
		class ABat* activeOverlappedBat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "item | Combat")
		class ABat* equippedBat;
	

	//		TimeLineShit		//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline")
	class UTimelineComponent* aimTimeLine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline")
		class UTimelineComponent* aimTimeLineReverse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline")
		class UTimelineComponent* toggleTimeLine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline")
		class UTimelineComponent* toggleTimeLineReverse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline")
		class UTimelineComponent* sprintTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UCurveFloat* fCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UCurveFloat* fCurve2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		class UCurveFloat* fCurve3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timeline)
	float startArmLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline)
		float startControlRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	float direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
		FRotator deltaDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float freezeCapsuleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timeline)
	float aimArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timeline)
	float toggleArmLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	float neckRotYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	float neckRotPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	FVector aimOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	FVector toggleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		FRotator cameraSprintFixup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	FVector cameraOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	FRotator toggleROffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	FRotator aimROffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	FRotator cameraROffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timeline")
	FRotator sprintControllerRot;




	

	/// visuals and collisions
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh")
	class USkeletalMeshComponent* headSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh")
	class USkeletalMeshComponent* clothingSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Static Mesh")
	UStaticMeshComponent* bombsStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	class UBoxComponent* kickCollision;


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
		class USphereComponent* range;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyCollision")
		class UBoxComponent* interactionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "step | snow")
		class UMaterialInterface* snowStepDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "step | inside")
		class UMaterialInterface* insideStepDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterialInterface* frozenMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterialInterface* regularMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FSlateBrush imageBrush{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class UImage* image;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class UNiagaraSystem* screamVFX;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "step | inside")
		class USoundCue* insideStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "step | inside")
		class UNiagaraSystem* insideStepParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | hit")
		class UNiagaraSystem* hitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue* hitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue* kickedSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Sound")
		class USoundBase* pickUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Sound")
		class USoundBase* parryingSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Sound")
		class USoundBase* parrySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundAttenuation* LoudAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundAttenuation* quietAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundConcurrency* concurrency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "step | snow")
		class USoundCue* snowStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "step | snow")
		class UNiagaraSystem* snowStepParticle;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AMainPlayerController* mainPlayerController;


protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Unfreeze();

	void BombSpawn();


	void ESCDown();

	UFUNCTION(BlueprintCallable)
	void DodgeAndSprint();

	void StopSprint();

	void Interact();

	void Freeze();

	void Attack();

	void PickUp();

	void Kick();

	UFUNCTION(BlueprintCallable)
	void Throw();

	void ToggleOn();

	void ToggleOff();

	void ToggleLeft();

	void ToggleRight();

	void ToggledCamera();
	//UFUNCTION(BlueprintImplementableEvent)
	void AimOn();

	void AimOff();

	UFUNCTION(BlueprintCallable)
	void SetupKickCollision(bool activate);

	UFUNCTION(BlueprintCallable)
	void RealSpawnAxe();

	UFUNCTION(BlueprintCallable)
	void StartGame();
	

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void RangeOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;
	
	UFUNCTION()
	void RangeOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;

	UFUNCTION()
	void DialogueOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void DialogueOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void KickOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE void SetOverlappingAxe(class AAxe* axe, bool axePull) { activeOverlappedAxe = axe; headAxe = axePull; }

	FORCEINLINE void SetEquippedAxe(class AAxe* axe) { equippedAxe = axe; }

	FORCEINLINE void SetOverlappingBat(class ABat* axe) { activeOverlappedBat = axe; }

	FORCEINLINE void SetOverlappingPickUp(AStaticPickUp* overlappedStatic, bool pickup, bool isBomb) { activeOverlappedStatic = overlappedStatic; bOverlappingPickup = pickup; bPickUpBomb = isBomb; }

	FORCEINLINE void SetEquippedBat(class ABat* axe) { equippedBat = axe; }

	float GetWalkingDirectionAngle();

	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void Stumbling();

	UFUNCTION()
		void BatHit();

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION()
	void StopDialogue();

	UFUNCTION()
	void StartDialogue();

	UFUNCTION()
		void AimTimeLineFloatReturn(float value);

	UFUNCTION()
		void AimReverseTimeLineFloatReturn(float value);

	UFUNCTION()
		void OnAimTimelineFinished();

	UFUNCTION()
		void OnSprintTimelineFinished();

	UFUNCTION()
		void ToggledTimeLineFloatReturn(float value);

	UFUNCTION()
		void ToggledReverseTimeLineFloatReturn(float value);

	UFUNCTION(BlueprintCallable)
		void AxePickUp();

	UFUNCTION(BlueprintCallable)
		void BatPickUp();

	UFUNCTION(BlueprintCallable)
		void SprintCamera(bool first);


	UFUNCTION(BlueprintCallable)
		void SprintTimeLineFloatReturn(float value);

	UFUNCTION(BlueprintCallable)
		void OnToggledTimelineFinished();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void NeckRotator();

	UFUNCTION()
	void DestroyAndSpawnEnemies();

	UFUNCTION()
	void Resurrect();


};

