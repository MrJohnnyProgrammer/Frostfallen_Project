// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Bear.generated.h"

/**
 * 
 */
UCLASS()
class FROSTFALLEN_API ABear : public AEnemy
{
	GENERATED_BODY()

public:


		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		AAxe* headAxe;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bRotate;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bPhase2;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bPhase3;
	
protected:

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		// Called every frame
		virtual void Tick(float DeltaTime) override;

		UFUNCTION(BlueprintCallable)
		void CloseRange();
		
		UFUNCTION(BlueprintCallable)
		void Charge();

		UFUNCTION(BlueprintCallable)
		void BackAOE();

		UFUNCTION(BlueprintCallable)
		void Stomp();

		UFUNCTION(BlueprintCallable)
			void Turn(bool bDirection);

		UFUNCTION(BlueprintCallable)
			void HeadShot();

		UFUNCTION(BlueprintCallable)
			void GotHit();

		UFUNCTION(BlueprintCallable)
			void Transition();

		UFUNCTION(BlueprintCallable)
			void DistanceAOE();

};
