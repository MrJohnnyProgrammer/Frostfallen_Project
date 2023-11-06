// Fill out your copyright notice in the Description page of Project Settings.


#include "Thrower.h"
#include "Components/BoxComponent.h"
#include "Axe.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"

void AThrower::BeginPlay()
{
	//bStrafing = true;	
	bThrower = true;
	Super::BeginPlay();
	health = 3;
	animInstance = GetMesh()->GetAnimInstance();
	bDead = false;
//	RealSpawnAxe();	

}
// Called every frame

void AThrower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	if (health < 1 && !dead)
	{
		if (equippedAxe)
			equippedAxe->Drop();

		Death();
		bPickedStrafeDirection = false;
		//	dead = true;
			//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead); //Die
	}
}


void AThrower::Stumble()
{
	//SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

	if (animInstance && !dead)
	{
		//	UE_LOG(LogTemp, Warning, TEXT("Debug"));
		bFrozen = false;
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("headAxe"));
		GetMesh()->SetMaterial(0, regularMaterial);
	}
}

void AThrower::Throw(FVector throwLocation)
{
//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

	if (animInstance && bAxeEquipped && !dead)
	{
		throwingLoc = throwLocation;
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("throw"));
	}
}

void AThrower::RealThrow()
{
	if (bAxeEquipped && equippedAxe && !dead)
	{
		
		//equippedAxe

		FVector startLocation = GetCapsuleComponent()->GetForwardVector() * 50 + GetCapsuleComponent()->GetUpVector() * 40 + GetMesh()->GetSocketLocation(FName("head"));
		//FVector endLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
		FVector endLocation = throwingLoc;

		FHitResult hit;
		FCollisionQueryParams params;
		FCollisionResponseParams rParams;
		params.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(hit, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params, rParams);
		//DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, true, 0.f, 0, 2.0f);
		//UE_LOG(LogTemp, Warning, TEXT("throw player: %f %f %f"), endLocation.X, endLocation.Y, endLocation.Z);
	
		if (hit.bBlockingHit)	
		{
		//	UE_LOG(LogTemp, Warning, TEXT("throw"));
			
			equippedAxe->Thrown(hit.ImpactPoint - startLocation, GetActorRotation(), true, startLocation);
			bAxeEquipped = false;
			axeOnHand(false);
		}
		else
		{
		//_	UE_LOG(LogTemp, Warning, TEXT("throw 2"));	
			
			equippedAxe->Thrown((endLocation - startLocation), GetActorRotation(), true, startLocation);
			bAxeEquipped = false;
			axeOnHand(false);
		}
	}
}

void AThrower::Kick()
{
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);


	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

	if (animInstance && !dead)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("kick"));

	}
}


void AThrower::Death()
{
	if (animInstance && !dead)
	{
		GetMesh()->bPauseAnims = false;
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
	//	UE_LOG(LogTemp, Warning, TEXT("RIP dude"));
		SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead); //Die
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//bNotDead = false;
		if (equippedAxe)
		{
			equippedAxe->Drop();
		}
		animInstance->Montage_Play(attackMontage, 1.f);
		GetMesh()->SetMaterial(0, regularMaterial);
		animInstance->Montage_JumpToSection(FName("death"));
		dead = true;
	}
}

void AThrower::PullAxe()
{
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	if (animInstance && !bAxeEquipped && !dead)
	{	
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("spawn"));
		axeOnHand(true);
	}
}

void AThrower::SpawnAxe()
{
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	if (animInstance && !bAxeEquipped && !dead)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("spawn"));
		axeOnHand(true);
	}
}


void AThrower::RealSpawnAxe()
{
	if (bAxeOnHead && headAxe)
	{
		GetMesh()->SetMaterial(0, frozenMaterial);

		headAxe->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		equippedAxe = headAxe ;
		equippedAxe->Equip(this, false);
		equippedAxe->bOnHead = false;
		bAxeOnHead = false;
		bFrozen = true;
		bAxeEquipped = true;
	//	UE_LOG(LogTemp, Warning, TEXT("Axe Pulled"));
	}
	else
	{
		FActorSpawnParameters spawnInfo;
		AActor* newAxe = GetWorld()->SpawnActor<AActor>(weaponBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
		equippedAxe = Cast<AAxe>(newAxe);
		equippedAxe->Equip(this, false);
		equippedAxe->bspawned = true;
		equippedAxe->bPermaSpawned = true;
		equippedAxe->skeletalMesh->SetMaterial(0, frozenMaterial);
		equippedAxe->health = 1;
		bAxeEquipped = true;
	//	UE_LOG(LogTemp, Warning, TEXT("Axe Spawned"));
	}
	
}

void AThrower::BodyThrowParry()
{
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

	if (animInstance && !dead)
	{
	//	UE_LOG(LogTemp, Warning, TEXT("parrydebug"));
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("parry"));
	}
}
