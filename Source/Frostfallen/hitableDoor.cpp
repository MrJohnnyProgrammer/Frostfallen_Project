// Fill out your copyright notice in the Description page of Project Settings.


#include "hitableDoor.h"
#include "Components/StaticMeshComponent.h"
#include "DefaultUnreal5Character.h"
#include "Kismet/GameplayStatics.h"
#include "Axe.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"

// Sets default values
AhitableDoor::AhitableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sMesh"));
	skeletalMesh->SetWorldLocation(FVector(0,0,30));
	skeletalMesh->SetWorldRotation(FRotator(-110, 180, 0));
	skeletalMesh->SetupAttachment(staticMesh);

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	staticMesh->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	staticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	staticMesh->SetSimulatePhysics(false);
	staticMesh->SetLinearDamping(2.f);
	staticMesh->SetAngularDamping(1.f);
//	staticMesh->SetupAttachment(minecraft);
	SetRootComponent(staticMesh);

	bFrozen = false;
	bShearable = false;
	bDead = false;
	bSpawnShearingAxe = false;
}

// Called when the game starts or when spawned
void AhitableDoor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AhitableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDead)
	{
		Destroy();
	}
}


float AhitableDoor::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *EventInstigator->GetPawn()->GetName());
	//1 = attack, 2 = throw, 3 = pullback after parry and also kick, 4 = FREEZE , 9 = Pull Damage
	
	AAxe* hitAxe = Cast<AAxe>(DamageCauser);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *DamageCauser->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("parry %f frozen"), Damage);
	if (bFrozen)
	{
		if (Damage == 2)
		{
		//	bFrozen = false;
			doorAxe = Cast<AAxe>(DamageCauser);
			doorAxe->DoorAxeAttach(this);
			bFrozen = false;
	//		UE_LOG(LogTemp, Warning, TEXT("parry %f frozen"), Damage);
			return 0;
		}
	}
	else
	{
		if (Damage == 1 || Damage == 7 || Damage == 8 || Damage == 9)
		{

			if (doorAxe)
			{
				if(Damage != 9)
					doorAxe->Drop();
			}


			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), doorBreakSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
			bDead = true;

			Destroy();
		}
		else if ( Damage == 2)
		{
		
				if (doorAxe )
				{
					doorAxe->Drop();
				}
						
			if(doorBreakSound)
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), doorBreakSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
	//		UE_LOG(LogTemp, Warning, TEXT("parry %f not frozen"), Damage);
			hitAxe->Drop();
			bDead = true;
			Destroy();

			return 0;
		}
		return 0;
	}
	return 0;
}
