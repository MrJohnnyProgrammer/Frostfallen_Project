// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomber.h"
#include "IceBomb.h"
#include "DefaultUnreal5Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"

void ABomber::BeginPlay()
{
	Super::BeginPlay();

	health = 1;
	bThrower = true;
	bBomber = true;
	animInstance = GetMesh()->GetAnimInstance();
	bDead = false;
	bFrozen = false;
}
// Called every frame

void ABomber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (health <= 0 && enemyMovementStatus != EEnemyMovementStatus::EMS_Dead && !bDead)
	{
	//	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Death();
		if (equippedBomb)
		{
			RealThrow();
		}
	}
}

void ABomber::Throw(FVector throwLocation)
{
//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

	if (animInstance && enemyMovementStatus != EEnemyMovementStatus::EMS_Dead && !bDead)
	{
		throwingLoc = throwLocation;
		RealSpawnBomb();
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("throw"));
	}
}

void ABomber::RealThrow()
{

		FVector startLocation = GetCapsuleComponent()->GetForwardVector() * 50 + GetCapsuleComponent()->GetUpVector() * 40 + GetMesh()->GetSocketLocation(FName("head"));
		FVector endLocation = throwingLoc;

		FHitResult hit;
		FCollisionQueryParams params;
		FCollisionResponseParams rParams;
		params.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(hit, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params, rParams);
		
	//	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 0.f, 0, 2.0f);
		//UE_LOG(LogTemp, Warning, TEXT("throw player: %f %f %f"), endLocation.X, endLocation.Y, endLocation.Z);

		if (hit.bBlockingHit)
		{
			equippedBomb->Thrown(hit.ImpactPoint - startLocation,GetActorRotation(), true);
		}
		else
		{
			equippedBomb->Thrown(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation() - startLocation, GetActorRotation(), true);
		}
}

void ABomber::RealSpawnBomb()
{
	if (enemyMovementStatus != EEnemyMovementStatus::EMS_Dead && !bDead)
	{
		FActorSpawnParameters spawnInfo;
		AActor* newBomb = GetWorld()->SpawnActor<AActor>(bombBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
		equippedBomb = Cast<AIceBomb>(newBomb);
		equippedBomb->EquipBomb(this);
	}
}



void ABomber::Death()
{
	
	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

	if (animInstance && enemyMovementStatus!=EEnemyMovementStatus::EMS_Dead)
	{
		bDead = true;
	//	UE_LOG(LogTemp, Warning, TEXT("RIP dude"));
		SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead); //Die
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//bNotDead = false;
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("death"));
	
	}
}


void ABomber::Stumble()
{
	//SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

	if (animInstance)
	{
		//	UE_LOG(LogTemp, Warning, TEXT("Debug"));
//		bFrozen = false;
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("headAxe"));
//		GetMesh()->SetMaterial(0, regularMaterial);
	}
}