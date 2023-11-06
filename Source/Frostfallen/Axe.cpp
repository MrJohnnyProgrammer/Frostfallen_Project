// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
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
#include "PhysicsEngine/BodyInstance.h"
#include "MainPlayerController.h"
#include "hitableDoor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ShearableDoor.h"

// Sets default values
AAxe::AAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bThrown = false;
	bOnHead = false;
	bDoor = false;
	health = 3;
	player = false;
	bPermaSpawned = false;
	bspawned = false;

		skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
		skeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		skeletalMesh->SetSimulatePhysics(false);
		skeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		skeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
		skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
		skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	//	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
		skeletalMesh->SetLinearDamping(2.f);
		skeletalMesh->SetAngularDamping(1.f);

		SetRootComponent(skeletalMesh);

		RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Component"));

		RotatingMovementComponent->SetUpdatedComponent(skeletalMesh);
		RotatingMovementComponent->bAutoActivate = false;
		RotatingMovementComponent->Deactivate();
		RotatingMovementComponent->bRotationInLocalSpace = true;
		RotatingMovementComponent->RotationRate = FRotator(-1500.f, 0.f,  0.f);


		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
		
		ProjectileMovementComponent->SetUpdatedComponent(skeletalMesh);
		ProjectileMovementComponent->bAutoActivate = false;
		ProjectileMovementComponent->bSimulationEnabled = false;
		ProjectileMovementComponent->Deactivate();
		ProjectileMovementComponent->InitialSpeed = 0.f;
		ProjectileMovementComponent->MaxSpeed = 6000.0f;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.1f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.2f;
		ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
		ProjectileMovementComponent->bSweepCollision = true;
	
	

	combatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Blade"));
	combatCollision->SetupAttachment(skeletalMesh);
	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	combatCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	combatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	combatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	combatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
//	combatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	combatCollision->AddRelativeLocation(FVector(10.f, 0.f, 40.f));
	combatCollision->SetBoxExtent(FVector(10.f, 2.f, 20.f));

	pickUpVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Pick-Up Volume"));
	pickUpVolume->SetupAttachment(skeletalMesh);
	pickUpVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	pickUpVolume->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	pickUpVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	pickUpVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	pickUpVolume->SetSphereRadius(100.f);
	pickUpVolume->AddRelativeLocation(FVector(0.f,0.f,20.f));
	
	
	pullCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PullAxe"));
	pullCollision->SetupAttachment(skeletalMesh);
	pullCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	pullCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	pullCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	pullCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	pullCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	pullCollision->AddRelativeLocation(FVector(-45.f, 0.f, -45.f));
	pullCollision->AddRelativeRotation(FRotator(25.f,0.f,0));
	pullCollision->SetBoxExtent(FVector(80.f, 50.f, 100.f));

	headCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("headCollision"));
	headCollision->SetupAttachment(skeletalMesh);
	headCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	headCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	headCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	headCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	headCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	headCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	headCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	headCollision->AddRelativeLocation(FVector(80.f, 0.f, 23.f));
	headCollision->AddRelativeRotation(FRotator(0.f, 0.f, 0));
	headCollision->SetBoxExtent(FVector(15.f, 5.f, 40.f));

}

// Called when the game starts or when spawned
void AAxe::BeginPlay()
{	
	Super::BeginPlay();
	

	pickUpVolume->OnComponentBeginOverlap.AddDynamic(this, &AAxe::pickUpOverlapBegin);
	pickUpVolume->OnComponentEndOverlap.AddDynamic(this, &AAxe::pickUpOverlapEnd);

	pullCollision->OnComponentBeginOverlap.AddDynamic(this, &AAxe::pullCollisionOverlapBegin);
	pullCollision->OnComponentEndOverlap.AddDynamic(this, &AAxe::pullCollisionOverlapEnd);

	combatCollision->OnComponentBeginOverlap.AddDynamic(this, &AAxe::BladeOverlapBegin);
	combatCollision->OnComponentEndOverlap.AddDynamic(this, &AAxe::BladeOverlapEnd);

	skeletalMesh->OnComponentHit.AddDynamic(this, &AAxe::OnBladeHit);

	

}

// Called every frame
void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bThrown && !(GetActorRotation().Roll > -50 && GetActorRotation().Roll < 50))
	{
		bNoisePlay = true;
	}

	if (bThrown && (GetActorRotation().Roll > -50 && GetActorRotation().Roll < 50))
	{
		if (bNoisePlay)
		{
			flyingSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), flySound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
			bNoisePlay = false;
		}
	}

	if (!bThrown)
	{
		bNoisePlay = false;

		if (flyingSound)
		{
			flyingSound->Stop();
		}
	}
	
}

void AAxe::LoseHealth()
{
	health -= 1;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, sparksVFX, combatCollision->GetComponentLocation(), FRotator(90.f, 0, 0));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, sparksVFX, combatCollision->GetComponentLocation(), FRotator(-90.f, 0, 0));

	if (health <= 0)
	{
		//	axeHolder.axe
	//	UE_LOG(LogTemp, Warning, TEXT("Axe destroyed"));
		if (Cast<ADefaultUnreal5Character>(axeHolder))
		{
			Cast<ADefaultUnreal5Character>(axeHolder)->bequippedWeapon = false;
			//Cast<ADefaultUnreal5Character>(axeHolder)->equippedAxe
			
			Cast<ADefaultUnreal5Character>(axeHolder)->equippedAxe = nullptr;
			Cast<ADefaultUnreal5Character>(axeHolder)->activeOverlappedAxe = nullptr;
		}

		if(bspawned)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), breakIceSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
		else
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), breakSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		Destroy();
	}
}

void AAxe::pickUpOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ADefaultUnreal5Character>(OtherActor)) {
		ADefaultUnreal5Character* playableCharacter = Cast<ADefaultUnreal5Character>(OtherActor);
		if (playableCharacter)
		{
			playableCharacter->SetOverlappingAxe(this, false);
			playableCharacter->bOverlappingPickup = true;
	
		}
	}
}
//pickUpOverlapEnd

void AAxe::pickUpOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ADefaultUnreal5Character>(OtherActor))
	{
		ADefaultUnreal5Character* playableCharacter = Cast<ADefaultUnreal5Character>(OtherActor);
		if (playableCharacter->mainPlayerController)
		{
			playableCharacter->SetOverlappingAxe(nullptr, false);
		}
	}
}

void AAxe::pullCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ADefaultUnreal5Character>(OtherActor))
	{

		ADefaultUnreal5Character* playableCharacter = Cast<ADefaultUnreal5Character>(OtherActor);

		if (playableCharacter->mainPlayerController)
		{

		//	UE_LOG(LogTemp, Warning, TEXT("overlapping"));
			playableCharacter->SetOverlappingAxe(this, true);
			playableCharacter->bPullingAxe = false;
			playableCharacter->bOverlappingPickup = true;
		}
	}
}

void AAxe::pullCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ADefaultUnreal5Character>(OtherActor))
	{
		ADefaultUnreal5Character* playableCharacter = Cast<ADefaultUnreal5Character>(OtherActor);
		if (playableCharacter)
		{
			playableCharacter->SetOverlappingAxe(nullptr, false);
			playableCharacter->bOverlappingPickup = false;
		}
	}
}

void AAxe::BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
//	AEnemy* character;

	if (OtherActor != axeHolder && OtherActor != this)
	{
	
		if (AAxe* anAxe = Cast<AAxe>(OtherActor))
		{
		
		}
		else
		{ 
		axeInstigator = axeHolder->GetInstigatorController();

			if (bThrown)
			{
			//	UE_LOG(LogTemp, Warning, TEXT("%s  "), *OtherActor->GetName());
				if (ACharacter * character = Cast<ACharacter>(OtherActor))
				{
					if (flyingSound)
					{
						flyingSound->Stop();
					}

					UGameplayStatics::ApplyDamage(OtherActor, 2, axeInstigator, this, damageTypeClass);
					Drop();

					DeactivateBladeCollision();
				
					LoseHealth();
					
				}	

			}
			else
			{
				UGameplayStatics::ApplyDamage(OtherActor, 1, axeInstigator, this, damageTypeClass);
			//	UE_LOG(LogTemp, Warning, TEXT("%s 2"), *OtherActor->GetName());
				DeactivateBladeCollision();

				UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAtten, concurrency, false);

				if (player)
					LoseHealth();
				
			}

		DeactivateBladeCollision();		
		}
	}

}

void AAxe::BladeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp)
	{
		ADefaultUnreal5Character* playableCharacter = Cast<ADefaultUnreal5Character>(OtherActor);
		if (playableCharacter)
		{

			playableCharacter->bOverlappingPickup = false;

		}
	}
}

void AAxe::Equip(ACharacter* playableCharacter, bool isPlayer)
{
	if (playableCharacter)
	{
		player = isPlayer;
		SetWeaponInstigator(playableCharacter->GetController());
		RotatingMovementComponent->Deactivate();
		ProjectileMovementComponent->Deactivate();
		skeletalMesh->SetSimulatePhysics(false);

		skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		pickUpVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		pullCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		headCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bspawned = false;

		bThrown = false;

		const USkeletalMeshSocket* rightHandSocket = playableCharacter->GetMesh()->GetSocketByName("RightHandSo");
		
		
		if (rightHandSocket)
		{
		//	UE_LOG(LogTemp, Warning, TEXT("attached"));
			rightHandSocket->AttachActor(this, playableCharacter->GetMesh());
			SetAxeHolder(playableCharacter);
		}

		if (equipSound)
		{

			UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), equipSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAtten, concurrency, false);
		}
	}
}

void AAxe::ActivateBladeCollision()
{
	if (combatCollision)
	{
		combatCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//	UE_LOG(LogTemp, Warning, TEXT("Swing"));
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAtten, concurrency, false);
	}
}

void AAxe::DeactivateBladeCollision()
{
	if (combatCollision )
		combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAxe::OnBladeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *OtherActor->GetName());

	if(bThrown)
	{
		if (OtherActor) 
		{
			 if (ACharacter* character = Cast<ACharacter>(OtherActor) )
			{
				 if (character != axeHolder)
				 {	
					 axeInstigator = axeHolder->GetInstigatorController();
					 if (bThrown)
					 {
						 UGameplayStatics::ApplyDamage(OtherActor, 2, axeInstigator, this, damageTypeClass);

						 combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						 bThrown = false;

						 DeactivateBladeCollision();

						 if (flyingSound)
						 {
							 flyingSound->Stop();
						 }

						 UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitThrowSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
						 Drop();
						 LoseHealth();
					 }

				 }
			}
			//Hit axe or door
			else if (Cast<AActor>(OtherActor))
			{
				 if (OtherActor != this)
				 {
					 //Hit Another Axe
					 if (Cast<AAxe>(OtherActor))
					 {

						 UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitThrowSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
						 UGameplayStatics::ApplyDamage(OtherActor, 2, axeInstigator, this, damageTypeClass);

						 if (flyingSound)
						 {
							 flyingSound->Stop();
						 }
						 Drop();
						 LoseHealth();
						 bThrown = false;
					
					 }
					 //Hit door
					 else  if (Cast<AhitableDoor>(OtherActor))
					 {
						 UE_LOG(LogTemp, Warning, TEXT("Hit hitable door"));
							 UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitThrowSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
							 UGameplayStatics::ApplyDamage(OtherActor, 2, axeInstigator, this, damageTypeClass);

							 if (flyingSound)
							 {
								 flyingSound->Stop();
							 }
							 Drop();
							 LoseHealth();
							 bThrown = false;
					 }
					 else if (Cast<AShearableDoor>(OtherActor)	)
					 {
						 UE_LOG(LogTemp, Warning, TEXT("Hit shear door"));
						 RotatingMovementComponent->Deactivate();
						 ProjectileMovementComponent->Deactivate();

						 UGameplayStatics::ApplyDamage(OtherActor, 2, axeInstigator, this, damageTypeClass);
						 FVector vector = Hit.ImpactNormal;
						 vector.Normalize(0.01);
						 vector = vector * -1;

						 FRotator rotation = vector.Rotation();

						 SetActorRotation(FRotator(rotation.Pitch - 30, playerRot.Yaw, rotation.Roll + 30));

						 bThrown = false;

						 UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitThrowSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

						 if (flyingSound)
						 {
							 flyingSound->Stop();
						 }
						 skeletalMesh->SetSimulatePhysics(false);
						 skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						 combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						 headCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

					 }
					 else
					 {
						 RotatingMovementComponent->Deactivate();
						 ProjectileMovementComponent->Deactivate();

						 SetActorLocation(Hit.ImpactPoint + Hit.ImpactNormal * 30);

						 UGameplayStatics::ApplyDamage(OtherActor, 2, axeInstigator, this, damageTypeClass);
						 FVector vector = Hit.ImpactNormal;
						 vector.Normalize(0.01);
						 vector = vector * -1;

						 FRotator rotation = vector.Rotation();

						 SetActorRotation(FRotator(rotation.Pitch - 30, playerRot.Yaw, rotation.Roll + 30));

						 bThrown = false;

						 if (flyingSound)
						 {
							 flyingSound->Stop();
						 }

						 UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitThrowSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);
				
						 skeletalMesh->SetSimulatePhysics(false);
						 skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						 pickUpVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
						 combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						 headCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						 LoseHealth();
					 }		
				 }	
				
			}
		}
		else if (OtherComp)
		{
			RotatingMovementComponent->Deactivate();
			ProjectileMovementComponent->Deactivate();

			SetActorLocation(Hit.ImpactPoint + Hit.ImpactNormal * 30);
	
			FVector vector = Hit.ImpactNormal;
			vector.Normalize(0.01);
			vector = vector * -1;

			FRotator rotation = vector.Rotation();

			SetActorRotation(FRotator(rotation.Pitch-30, playerRot.Yaw, rotation.Roll));

			if (flyingSound)
			{
				flyingSound->Stop();
			}

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitThrowSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

			skeletalMesh->SetSimulatePhysics(false);
			skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			pickUpVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			headCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			LoseHealth();
		}
	}
}

void AAxe::Thrown(const FVector &direction,  FRotator throwRot, bool isAi, FVector start)
{
	float throwYaw;
	float throwPitch;
	float throwRoll;
	playerRot = throwRot;
	bThrown = true;

	FTimerHandle UnusedHandle;

	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorLocation(start);
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
	//combatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	headCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	headCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	headCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (bPermaSpawned)
		bspawned = true;

	if (bspawned)
	{
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AAxe::DeSpawn, 5.f, false);
	}
}

void AAxe::DeSpawn()
{
	if(bspawned)
		Destroy();
}

void AAxe::AllowCollision()
{
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//UE_LOG(LogTemp, Warning, TEXT("throw2"));	

}

void AAxe::DoorAxeAttach(AhitableDoor* door)
{
	skeletalMesh->SetSimulatePhysics(false);
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	pickUpVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	pullCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	headCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RotatingMovementComponent->Deactivate();
	ProjectileMovementComponent->Deactivate();

	bDoor = true;
	axeDoorHolder = door;
	
	if (axeDoorHolder->bSpawnShearingAxe)
	{
		AShearableDoor* newShearDoor = Cast<AShearableDoor>(door);
	}
}

void AAxe::HeadAxeAttach(AEnemy* enemy, FName socket)
{
	skeletalMesh->SetSimulatePhysics(false);
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	pickUpVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	pullCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	headCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RotatingMovementComponent->Deactivate();
	ProjectileMovementComponent->Deactivate();
	
	const USkeletalMeshSocket* headAxedSocket = enemy->headSkeletalMesh->GetSocketByName(socket);

	//UE_LOG(LogTemp, Warning, TEXT("DamageBaby4"));

	if (headAxedSocket)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Axe In Head"));
		headAxedSocket->AttachActor(this, enemy->headSkeletalMesh);
		bOnHead = true;
		SetAxeHolder(enemy);
		axeHeadHolder = enemy;
	//	axeHeadHolder->equippedAxe = this;

	}
}

void AAxe::PullAxeFromHead(AController* instigrator)
{
	if (axeHeadHolder || axeDoorHolder)
	{
		if (!bearAss)
		{
			if (bOnHead)
			{
				UGameplayStatics::ApplyDamage(axeHeadHolder, 1, instigrator, this, damageTypeClass);
				LoseHealth();
				bOnHead = false;
				axeHeadHolder = nullptr;
			//	LoseHealth();
			}
			else if (bDoor)
			{
				axeDoorHolder->bFrozen = false;
				UGameplayStatics::ApplyDamage(axeDoorHolder, 9, instigrator, this, damageTypeClass);
				LoseHealth();
				bDoor = false;
				axeDoorHolder = nullptr;
			
			}

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), equipSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		}
		else 
		{
		bearAss = false;
		}
		
	}

}

void AAxe::PullAxeAnimation()
{
	if (bOnHead)
	{

		axeHeadHolder->PlayerPullAxe();

	}
}

void AAxe::Drop()
{

	if (RotatingMovementComponent && RotatingMovementComponent->IsActive())
	{
//		RotatingMovementComponent->RotationRate = FRotator(0.f);
		RotatingMovementComponent->Deactivate();
	}

	if (ProjectileMovementComponent && ProjectileMovementComponent->IsActive())
	{
//		ProjectileMovementComponent->Velocity = FVector(0.f);
//		ProjectileMovementComponent->bSimulationEnabled = false;
		ProjectileMovementComponent->Deactivate();
	}

	if (Cast<ADefaultUnreal5Character>(axeHolder))
	{
		Cast<ADefaultUnreal5Character>(axeHolder)->equippedAxe = nullptr;
		Cast<ADefaultUnreal5Character>(axeHolder)->bequippedWeapon = false;

	}
	else if (Cast<AEnemy>(axeHolder))
	{
		Cast<AEnemy>(axeHolder)->equippedAxe = nullptr;
		Cast<AEnemy>(axeHolder)->bAxeEquipped = false;
	}

	pickUpVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	headCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	pullCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	skeletalMesh->SetSimulatePhysics(true);
	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	bThrown = false;
	//UE_LOG(LogTemp, Warning, TEXT("rip"));
}