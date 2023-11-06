// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"


/**
 *
 */
UCLASS()
class FROSTFALLEN_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
		virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
		void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float movementSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AimOffset)
		float aoPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IK)
		float feetDisplacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IK)
		FRotator LRot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IK)
		FRotator RRot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IK)
		float RIK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK")
		float LIK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "step")
		class ADecalActor* decalStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | hit")
		class USoundBase* stepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | hit")
		class USoundAttenuation* loudAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | hit")
		class USoundAttenuation* quietAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual | hit")
		class USoundConcurrency* concurrency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "step")
		class UNiagaraSystem* stepParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IK)
		float ikInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool bIsinAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool bLocked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class AEnemy* enemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class ANPC* npc;


public:
	UFUNCTION(BlueprintCallable)	
		void Displacement(FName socketName, float& outDistance, bool& outBool);

	UFUNCTION(BlueprintCallable)
		void LineTrace(FName socketName, float& outDistance, FVector& outNormal);

	UFUNCTION(BlueprintCallable)
		void FootPlacement();

	UFUNCTION(BlueprintCallable)
		void FootFX(bool leftFoot);

};

