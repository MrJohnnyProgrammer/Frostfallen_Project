// Fill out your copyright notice in the Description page of Project Settings.


#include "Marauder.h"
#include "Components/BoxComponent.h"
#include "Axe.h"
#include "Bat.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

void AMarauder::BeginPlay()
{
	Super::BeginPlay();
	animInstance = GetMesh()->GetAnimInstance();
//	bMagic = false;
	
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
	if (!bBatter)
	{
	//	bBatter = false;
		health = 3;
		bMarauder = true;
		FActorSpawnParameters spawnInfo;
		AActor* newAxe = GetWorld()->SpawnActor<AActor>(weaponBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
		equippedAxe = Cast<AAxe>(newAxe);
		equippedAxe->Equip(this,false);
		bAxeEquipped = true;
		bBasicAttackCD = false;
	}
}

// Called every frame
void AMarauder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (health < 1 && !dead)
	{
		if(equippedAxe)
			equippedAxe->Drop();

		Death();
		bPickedStrafeDirection = false;
	//	dead = true;
		//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead); //Die
	}

}

void AMarauder::JumpAttack()
{
//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
	bAttacking = true;
	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
	bPickedStrafeDirection = false;
	if (animInstance)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("jump"));
		bPickedStrafeDirection = false;
	}
}


void AMarauder::Kick()
{
	//SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);	
	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
	bAttacking = true;
	if (animInstance)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("kick"));
		bPickedStrafeDirection = false;
	}
}



void AMarauder::Attack()
{
//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
	bAttacking = true;
//	UGameplayStatics::PlaySoundAtLocation(GetWorld());
	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f,1.f,0.f, quietAttenuation,concurrency, false);



	if (animInstance)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("attack"));
		bPickedStrafeDirection = false;
	}
}

void AMarauder::SmackDown()
{
//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
	bAttacking = true;
	if (animInstance)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		if (!bBatter)
		{
			if (UKismetMathLibrary::RandomBool())
			{
				animInstance->Montage_JumpToSection(FName("smack"));

			}
			else
			{
				animInstance->Montage_JumpToSection(FName("runsmack"));
			}
		}
		else
		{
			animInstance->Montage_JumpToSection(FName("smack"));

		}
		
		
		bPickedStrafeDirection = false;

	}
}

void AMarauder::RunSmack()
{
	//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
	bAttacking = true;
	if (animInstance)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		
		bPickedStrafeDirection = false;

	}
}

void AMarauder::BackAttack()
{
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
	bAttacking = true;
	if (animInstance)
	{
		bPickedStrafeDirection = false;
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("back"));

	}
}

void AMarauder::RealSpawnAxe()
{
	FActorSpawnParameters spawnInfo;
	AActor* newAxe = GetWorld()->SpawnActor<AActor>(weaponBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
	equippedAxe = Cast<AAxe>(newAxe);
	equippedAxe->Equip(this, false);
	bAxeEquipped = true;
	equippedAxe->health = 1;
	equippedAxe->bspawned = true;
	equippedAxe->bPermaSpawned = true;
	equippedAxe->skeletalMesh->SetMaterial(0, frozenMaterial);
}

void AMarauder::SpawnAxe()
{
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	if (animInstance && !bAxeEquipped)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("spawn"));
		axeOnHand(true);

	}
}
void AMarauder::DownAttack()
{
	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
	bAttacking = true;
	if (animInstance)
	{
		bPickedStrafeDirection = false;
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("down"));

	}
}


void AMarauder::Stumble()
{
//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
	if (animInstance)
	{
		animInstance->StopAllMontages(1.f);
		bPickedStrafeDirection = false;
		bFrozen = false;
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("parried"));
		GetMesh()->SetMaterial(0, regularMaterial);
	}
}

void AMarauder::Scream()
{
	//SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);	
	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), screamSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, screamVFX, GetActorLocation(), FRotator(0.f));

	bAttacking = true;
	if (animInstance)
	{
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("scream"));
		bPickedStrafeDirection = false;
	}
}

void AMarauder::RealScream()
{
	//SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);	
	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), screamSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
		
	TArray<FHitResult> OutResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Reserve(2);
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle));
	//ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle));
	TArray<AActor*> ignoredActors;

	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), 300.f, ObjectTypesArray, false, ignoredActors, EDrawDebugTrace::None, OutResults, true);
	//UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint, 150.f, ObjectTypesArray, false, ignoredActors, EDrawDebugTrace::ForDuration, hit, true);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), OutResults.Num());
	for (int i = 0; i < OutResults.Num(); i++) {
		if (OutResults[i].bBlockingHit)
		{
			
			if (Cast<ACharacter>(OutResults[i].GetActor()))
			{
				if (Cast<AEnemy>(OutResults[i].GetActor()) != this)
					UGameplayStatics::ApplyDamage(OutResults[i].GetActor(), 6, OutResults[i].GetActor()->GetInstigatorController(), this, damageTypeClass);
			}
			
		}
		//UGameplayStatics::ApplyDamage(OutResults[i].GetActor(), 4, OutResults[i].GetActor()->GetInstigatorController() , this, damageTypeClass);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OutResults[i].GetActor()->GetName());
	}
}

void AMarauder::Death()
{

	if (animInstance && enemyMovementStatus != EEnemyMovementStatus::EMS_Dead && !dead)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
		bPickedStrafeDirection = false;
		dead = true;
	//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead); //Die
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//bNotDead = false;
		if (equippedAxe)
		{
			equippedAxe->Drop();
		}
		GetMesh()->SetMaterial(0, regularMaterial);
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("death"));
	}
}

void AMarauder::BodyThrowParry()
{
//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

	if (animInstance)
	{
	//	UE_LOG(LogTemp, Warning, TEXT("parrydebug"));
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("parry"));
	}
}