// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticPickUp.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DefaultUnreal5Character.h"

// Sets default values
AStaticPickUp::AStaticPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bMoon = false;
	bBomb = false;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	staticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	staticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	pickUpVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Pick-Up Volume"));
	pickUpVolume->SetupAttachment(staticMesh);
	pickUpVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	pickUpVolume->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	pickUpVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	pickUpVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	pickUpVolume->SetSphereRadius(100.f);
	pickUpVolume->AddRelativeLocation(FVector(0.f, 0.f, 20.f));
}

// Called when the game starts or when spawned
void AStaticPickUp::BeginPlay()
{
	Super::BeginPlay();

	pickUpVolume->OnComponentBeginOverlap.AddDynamic(this, &AStaticPickUp::pickUpOverlapBegin);
	pickUpVolume->OnComponentEndOverlap.AddDynamic(this, &AStaticPickUp::pickUpOverlapEnd);

	
}

void AStaticPickUp::pickUpOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		ADefaultUnreal5Character* playableCharacter = Cast<ADefaultUnreal5Character>(OtherActor);
		if (playableCharacter)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("overlapping"));
		//	playableCharacter->SetOverlappingAxe(this, false);
			//	playableCharacter->mainPlayerController->WPickU
				playableCharacter->SetOverlappingPickUp(this, true, bBomb);
				//	playableCharacter->mainPlayerController.
			//	playableCharacter->mainPlayerController->DisplayPickUp();
			
			//	playableCharacter->mainPlayerController->DisplayPickUp();

		//	playableCharacter->binAxeRange = true;
		}
	}
}

void AStaticPickUp::pickUpOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp) {
		ADefaultUnreal5Character* playableCharacter = Cast<ADefaultUnreal5Character>(OtherActor);
		if (playableCharacter->mainPlayerController)
		{
		//	playableCharacter->SetOverlappingAxe(nullptr, false);
			playableCharacter->SetOverlappingPickUp(nullptr, false, bBomb);
			//	playableCharacter->mainPlayerController->RemovePickUp();
			//	mainPlayerController->DisplayToggle();
		//	playableCharacter->mainPlayerController->RemovePickUp();
			//	playableCharacter->binAxeRange = false;
		}
	}
}

// Called every frame
void AStaticPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

