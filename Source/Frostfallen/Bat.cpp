// Fill out your copyright notice in the Description page of Project Settings.


#include "Bat.h"
#include "Enemy.h" 
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DefaultUnreal5Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/AudioComponent.h"
#include "hitableDoor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "MainPlayerController.h"

// Sets default values
ABat::ABat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bThrown = false;
	player = false;

	health = 3;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	skeletalMesh->SetSimulatePhysics(true);
	skeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	skeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	skeletalMesh->SetLinearDamping(2.f);
	skeletalMesh->SetAngularDamping(1.f);

	SetRootComponent(skeletalMesh);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Component"));

	RotatingMovementComponent->SetUpdatedComponent(skeletalMesh);
	RotatingMovementComponent->bAutoActivate = false;
	RotatingMovementComponent->Deactivate();
	RotatingMovementComponent->bRotationInLocalSpace = true;
	RotatingMovementComponent->RotationRate = FRotator(-2000.f, 0.f, 0.f);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));

	ProjectileMovementComponent->SetUpdatedComponent(skeletalMesh);
	ProjectileMovementComponent->bAutoActivate = false;
	ProjectileMovementComponent->bSimulationEnabled = false;
	ProjectileMovementComponent->Deactivate();
	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.1f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.2f;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
	ProjectileMovementComponent->bSweepCollision = true;

	pickUpVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Pick-Up Volume"));

	pickUpVolume->SetupAttachment(skeletalMesh);
	pickUpVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	pickUpVolume->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	pickUpVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	pickUpVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	pickUpVolume->SetSphereRadius(100.f);
	pickUpVolume->AddRelativeLocation(FVector(0.f, 0.f, 20.f));

	combatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Blade"));
	combatCollision->SetupAttachment(skeletalMesh);
	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	combatCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	combatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	combatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	combatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	combatCollision->AddRelativeLocation(FVector(0.f, 0.f, 60.f));
	combatCollision->SetBoxExtent(FVector(10.f, 10.f, 20.f));


}

// Called when the game starts or when spawned
void ABat::BeginPlay()
{
	Super::BeginPlay();
	
	pickUpVolume->OnComponentBeginOverlap.AddDynamic(this, &ABat::pickUpOverlapBegin);
	pickUpVolume->OnComponentEndOverlap.AddDynamic(this, &ABat::pickUpOverlapEnd);

	combatCollision->OnComponentBeginOverlap.AddDynamic(this, &ABat::BladeOverlapBegin);
	
	skeletalMesh->OnComponentHit.AddDynamic(this, &ABat::OnBladeHit);

	//skeletalMesh->SetSimulatePhysics(true);
}

// Called every frame
void ABat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!(GetActorRotation().Roll > -50 && GetActorRotation().Roll < 50))
	{
		bNoisePlay = true;
	}

	if (bThrown && (GetActorRotation().Roll > -50 && GetActorRotation().Roll < 50))
	{

		//		FTimerHandle UnusedHandle;
		//		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AAxe::Debug, 1.f, false);
		if (bNoisePlay)
		{
			flyingSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), flySound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
			bNoisePlay = false;
		}

	}
}

void ABat::pickUpOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ADefaultUnreal5Character>(OtherActor)) {
		ADefaultUnreal5Character* playableCharacter = Cast<ADefaultUnreal5Character>(OtherActor);
		if (playableCharacter && playableCharacter->mainPlayerController)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("overlapping"));
			playableCharacter->SetOverlappingBat(this);

			playableCharacter->bOverlappingPickup = true;
			//	playableCharacter->mainPlayerController->WPickU

			playableCharacter->mainPlayerController->pickUpLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 50);

		}
	}
}

void ABat::pickUpOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ADefaultUnreal5Character>(OtherActor)) {
		ADefaultUnreal5Character* playableCharacter = Cast<ADefaultUnreal5Character>(OtherActor);
		if (playableCharacter)
		{
			playableCharacter->SetOverlappingBat(nullptr);
			playableCharacter->binAxeRange = false;
			playableCharacter->bOverlappingPickup = false;
		}
	}
}

void ABat::BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//	AEnemy* character;

	if (OtherActor != batHolder && OtherActor != this)
	{
		batInstigator = batHolder->GetInstigatorController();

		if (bThrown)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bat throw hit"));
			UGameplayStatics::ApplyDamage(OtherActor, 7, batInstigator, this, damageTypeClass);
			//	Drop();
			//	
		//	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
			DeactivateBladeCollision();
		//	if (player)
			LoseHealth();
		}
		else
		{

			UGameplayStatics::ApplyDamage(OtherActor, 7, batInstigator, this, damageTypeClass);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
			DeactivateBladeCollision();
			if (player)
				LoseHealth();

			UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAtten, concurrency, false);

		}

		DeactivateBladeCollision();
	}
}

void ABat::Equip(ACharacter* playableCharacter, bool isPlayer)
{
	if (playableCharacter)
	{
		player = isPlayer;

		skeletalMesh->SetSimulatePhysics(false);
		SetWeaponInstigator(playableCharacter->GetController());
		RotatingMovementComponent->Deactivate();
		ProjectileMovementComponent->Deactivate();
		

		skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		pickUpVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	ProjectileMovementComponent->bSimulationEnabled = false;
		bThrown = false;
		
		const USkeletalMeshSocket* rightHandSocket = playableCharacter->GetMesh()->GetSocketByName("RightHandSo");

		if (rightHandSocket)
		{
			rightHandSocket->AttachActor(this, playableCharacter->GetMesh());
			skeletalMesh->SetSimulatePhysics(false);
			SetAxeHolder(playableCharacter);
		}

		if (equipSound)
		{

			UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), equipSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAtten, concurrency, false);
		}
	}
}

void ABat::ActivateBladeCollision()
{
	if (combatCollision)
	{
		combatCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//	UE_LOG(LogTemp, Warning, TEXT("Swing"));
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAtten, concurrency, false);
	}
}

void ABat::DeactivateBladeCollision()
{
	if (combatCollision)
		combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABat::OnBladeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	if(bThrown)
	{
		batInstigator = batHolder->GetInstigatorController();
		if (OtherActor)
		{
			if (UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(OtherActor))
			{
				Drop();
				LoseHealth();
				DeactivateBladeCollision();
			}
			else if (ACharacter* character = Cast<ACharacter>(OtherActor))
			{
				if (character != batHolder)
				{
					if (batInstigator)
						UGameplayStatics::ApplyDamage(OtherActor, 7, batInstigator, this, damageTypeClass);

					if (flyingSound)
					{
						flyingSound->Stop();
					}
					LoseHealth();
					Drop();
					DeactivateBladeCollision();
				}
			}

			else if (Cast<AhitableDoor>(OtherActor))
			{
				//	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
				RotatingMovementComponent->Deactivate();
				ProjectileMovementComponent->Deactivate();

				//	 SetActorLocation(Hit.ImpactPoint + Hit.ImpactNormal * 30);//SetActorLocation(Hit.ImpactPoint + Hit.ImpactNormal * 30);
				 //	Hit.ImpactNormal.Rotation();

				//	 DrawDebugSphere(GetWorld(), Hit.ImpactPoint + Hit.ImpactNormal * 30, 10, 8, FColor(181, 0, 0), true, -1, 0, 2);
				UGameplayStatics::ApplyDamage(OtherActor, 7, batInstigator, this, damageTypeClass);
				LoseHealth();
				Drop();
			//	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
				DeactivateBladeCollision();
			}
			else
			{
				LoseHealth();
				Drop();
			//	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
				DeactivateBladeCollision();
			}
		}
		else
		{
			LoseHealth();
			Drop();
		//	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
			DeactivateBladeCollision();
		}
	}
}

void ABat::Thrown(const FVector& direction, FRotator throwRot, bool isAi, FVector start)
{

	float throwYaw;
	float throwPitch;
	float throwRoll;
	playerRot = throwRot;
	bThrown = true;

	FTimerHandle UnusedHandle;
	//GetWorldTimerManager().SetTimer(UnusedHandle, this, &AAxe::AllowCollision, 0.1f, false);

	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorLocation(start);
	//SetActorRotation(playerRot);
//	SetActorLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FollowCamera->GetForwardVector() * 50 + UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FollowCamera->GetComponentLocation())
	if (isAi)
	{
		FVector throwDirection = direction * 2.0;

		throwYaw = direction.Z / throwDirection.Size();
		throwPitch = direction.Y / throwDirection.Size();
		throwRoll = direction.X / throwDirection.Size();

		throwYaw *= 4000.f;
		throwPitch *= 4000.f;
		throwRoll *= 4000.f;

	}
	else
	{
		FVector throwDirection = direction * 2.0;

		throwYaw = direction.Z / throwDirection.Size();
		throwPitch = direction.Y / throwDirection.Size();
		throwRoll = direction.X / throwDirection.Size();

		throwYaw *= 4000.f;
		throwPitch *= 4000.f;
		throwRoll *= 4000.f;
	}

	//UE_LOG(LogTemp, Warning, TEXT("provoke"));
	ProjectileMovementComponent->Velocity = FVector(throwRoll, throwPitch, throwYaw);
	thrownDirection = ProjectileMovementComponent->Velocity;
	RotatingMovementComponent->Activate();
	ProjectileMovementComponent->Activate();

	//UE_LOG(LogTemp, Warning, TEXT("throw player: %f %f %f"), throwRoll, throwPitch, throwYaw);
	ProjectileMovementComponent->bSimulationEnabled = true;

	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);

	SetActorRotation(FRotator(playerRot.Pitch, playerRot.Yaw, playerRot.Roll + 20));
	ActivateBladeCollision();

	pickUpVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	combatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


}

void ABat::AllowCollision()
{
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ABat::Drop()
{
	if (RotatingMovementComponent->IsActive())
	{
		//		RotatingMovementComponent->RotationRate = FRotator(0.f);
		RotatingMovementComponent->Deactivate();
	}

	if (ProjectileMovementComponent->IsActive())
	{
		//		ProjectileMovementComponent->Velocity = FVector(0.f);
		//		ProjectileMovementComponent->bSimulationEnabled = false;
		ProjectileMovementComponent->Deactivate();
	}


	pickUpVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	skeletalMesh->SetSimulatePhysics(true);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	bThrown = false;
//	UE_LOG(LogTemp, Warning, TEXT("rip"));
}


void ABat::LoseHealth()
{
	health -= 1;


	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, sparksVFX, combatCollision->GetComponentLocation(), FRotator(90.f, 0, 0));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, sparksVFX, combatCollision->GetComponentLocation(), FRotator(-90.f, 0, 0));

	if (health <= 0)
	{
		//	axeHolder.axe
	//	UE_LOG(LogTemp, Warning, TEXT("Axe destroyed"));
		if (Cast<ADefaultUnreal5Character>(batHolder))
		{
			Cast<ADefaultUnreal5Character>(batHolder)->bequippedWeapon = false;
			Cast<ADefaultUnreal5Character>(batHolder)->equippedBat = nullptr;
			Cast<ADefaultUnreal5Character>(batHolder)->activeOverlappedBat = nullptr;

		}

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), breakSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		Destroy();
	}
}