// Fill out your copyright notice in the Description page of Project Settings.


#include "Batter.h"
#include "Bat.h"

void ABatter::BeginPlay()
{
	
	animInstance = GetMesh()->GetAnimInstance();
	bMagic = false;
	bBatter = true;

	FActorSpawnParameters spawnInfo;
	health = 2;
	AActor* newBat = GetWorld()->SpawnActor<AActor>(weaponBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
	equippedBat = Cast<ABat>(newBat);
	equippedBat->Equip(this, false);
	bAxeEquipped = true;

	Super::BeginPlay();

	
}

// Called every frame
void ABatter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (health < 1 && !dead)
	{
		if (equippedBat)
			equippedBat->Drop();

		Death();
		bPickedStrafeDirection = false;
		//	dead = true;
			//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead); //Die
	}

}