// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicMarauder.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Axe.h"

void AMagicMarauder::BeginPlay()
{
	Super::BeginPlay();

	animInstance = GetMesh()->GetAnimInstance();
	dead = false;
	bMagic = true;
	health = 4;
	//	RealSpawnAxe();	

}
// Called every frame

void AMagicMarauder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (health <= 0)
	{
		Death();
	}
}



void AMagicMarauder::RealThrow()
{

	if (bAxeEquipped)
	{

		//equippedAxe

		FVector startLocation = GetMesh()->GetSocketLocation(FName("RightHandSo"));
		FVector endLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();

		FHitResult hit;
		FCollisionQueryParams params;
		FCollisionResponseParams rParams;
		params.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(hit, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params, rParams);
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, true, 0.f, 0, 2.0f);
		//UE_LOG(LogTemp, Warning, TEXT("throw player: %f %f %f"), endLocation.X, endLocation.Y, endLocation.Z);

		if (hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("throw"));
			equippedAxe->Thrown(hit.ImpactPoint - startLocation, GetActorRotation(), true, startLocation);
			bAxeEquipped = false;
			axeOnHand(false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("throw 2"));

			equippedAxe->Thrown((UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation() - startLocation), GetActorRotation(), true,startLocation);
			bAxeEquipped = false;
			axeOnHand(false);
		}
	}
}

void AMagicMarauder::Throw()
{
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

	if (animInstance)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("throw"));
	}
}

void AMagicMarauder::Taunt()
{
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

	if (animInstance)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("taunt"));
	}
}

