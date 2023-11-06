// Fill out your copyright notice in the Description page of Project Settings.


#include "ShearableDoor.h"
#include "Axe.h"

void AShearableDoor::BeginPlay()
{
	Super::BeginPlay();

	bFrozen = true;
	bShearable = true;

	if (bSpawnShearingAxe)
	{
		FActorSpawnParameters spawnInfo;
		AActor* newAxe = GetWorld()->SpawnActor<AActor>(weaponBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
		doorAxe = Cast<AAxe>(newAxe);

		doorAxe->DoorAxeAttach(this);
		
		doorAxe->SetActorRotation(FRotator(GetActorRotation().Pitch - 40, GetActorRotation().Roll - 70, GetActorRotation().Yaw - 90));
	//	doorAxe->SetActorRotation(skeletalMesh->GetComponentRotation());
	//	doorAxe->SetActorLocation(skeletalMesh->GetComponentLocation());
	///	doorAxe->SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Roll, GetActorRotation().Yaw +90));
		doorAxe->SetActorLocation(FVector(GetActorLocation().X + 0, GetActorLocation().Y + 50, GetActorLocation().Z + 30.f));

		bFrozen = false;
	}
}