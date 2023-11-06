// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h" 
#include "Axe.h"
#include "Bat.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "DefaultUnreal5Character.h"
#include "EngineUtils.h"
#include "NavigationSystemTypes.h"
#include "NavigationSystem.h"
#include "NavigationData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Sound/SoundAttenuation.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundConcurrency.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	rotationTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("rotation timeline"));

	navPathIndex = 0;
	bStrafing = false;
	health = 1;
	bear = false;
	bThrower = false;
	bBomber = false;
	bDead = false;

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	headSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Mesh"));
	headSkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	headSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	headSkeletalMesh->SetupAttachment(GetMesh());

	clothingSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Clothing Mesh"));
	clothingSkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	clothingSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	clothingSkeletalMesh->SetupAttachment(GetMesh());

	combatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("box"));
	combatCollision->SetupAttachment(GetMesh());
	combatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("foot_l"));
	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	combatCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	combatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

	combatCollision->SetRelativeLocation(FVector(15, 5.f, 0.f)); //15,5,0
	combatCollision->SetBoxExtent(FVector(25.f, 20.f, 20.f));
	combatCollision->bHiddenInGame = false;

	FRotator NineOnZ = FRotator(90.f, 0.f, 0.f);

	bAttacking = false;
/*
	torso = CreateDefaultSubobject<UCapsuleComponent>(FName("torso"));
	torso->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("spine_03"));
	torso->SetRelativeLocation(FVector(7.f, 0.f, 0.f));
	torso->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	torso->InitCapsuleSize(15, 40);
	torso->CanCharacterStepUp(false);
	torso->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	torso->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	torso->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	torso->SetShouldUpdatePhysicsVolume(true);

	head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("head"));
	head->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	head->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	head->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	head->InitCapsuleSize(15.f, 15.f);
	head->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("neck_02"));
	head->SetRelativeLocation(FVector(7.f, 0.f, 0.f));
	head->SetRelativeRotation(NineOnZ);
	head->InitCapsuleSize(15.f, 15.0f);
	head->CanCharacterStepUp(false);
	head->SetShouldUpdatePhysicsVolume(true);

	rDownArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("rDownArm"));
	rDownArm->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("lowerarm_r"));
	rDownArm->SetRelativeLocation(FVector(-16.f, 0.f, 0.f));
	rDownArm->SetRelativeRotation(NineOnZ);
	rDownArm->InitCapsuleSize(8.f, 20.f);
	rDownArm->CanCharacterStepUp(false);
	rDownArm->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rDownArm->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	rDownArm->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	rDownArm->SetShouldUpdatePhysicsVolume(true);

	rUpArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("rUpArm"));
	rUpArm->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("upperarm_r"));
	rUpArm->SetRelativeLocation(FVector(-12.f, 0.f, 0.f));
	rUpArm->SetRelativeRotation(NineOnZ);
	rUpArm->InitCapsuleSize(10.f, 20.f);
	rUpArm->CanCharacterStepUp(false);
	rUpArm->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rUpArm->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	rUpArm->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	rUpArm->SetShouldUpdatePhysicsVolume(true);

	lDownArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("lDownArm"));
	lDownArm->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("lowerarm_l"));
	lDownArm->SetRelativeLocation(FVector(16.f, 0.f, 0.f));
	lDownArm->SetRelativeRotation(NineOnZ);
	lDownArm->InitCapsuleSize(8.f, 20.f);
	lDownArm->CanCharacterStepUp(false);
	lDownArm->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	lDownArm->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	lDownArm->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lDownArm->SetShouldUpdatePhysicsVolume(true);

	lUpArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("lUpArm"));
	lUpArm->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("upperarm_l"));
	lUpArm->SetRelativeLocation(FVector(12.f, 0.f, 0.f));
	lUpArm->SetRelativeRotation(NineOnZ);
	lUpArm->InitCapsuleSize(10.f, 20.f);
	lUpArm->CanCharacterStepUp(false);
	lUpArm->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	lUpArm->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	lUpArm->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lUpArm->SetShouldUpdatePhysicsVolume(true);

	rDownLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("rDownLeg"));
	rDownLeg->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("calf_r"));
	rDownLeg->SetRelativeLocation(FVector(20.f, 0.f, 0.f));
	rDownLeg->SetRelativeRotation(NineOnZ);
	rDownLeg->InitCapsuleSize(8.f, 25.f);
	rDownLeg->CanCharacterStepUp(false);
	rDownLeg->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rDownLeg->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	rDownLeg->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	rDownLeg->SetShouldUpdatePhysicsVolume(true);

	rUpLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("rUpLeg"));
	rUpLeg->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("thigh_r"));
	rUpLeg->SetRelativeLocation(FVector(20.f, 0.f, 0.f));
	rUpLeg->SetRelativeRotation(NineOnZ);
	rUpLeg->InitCapsuleSize(10.f, 25.f);
	rUpLeg->CanCharacterStepUp(false);
	rUpLeg->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rUpLeg->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	rUpLeg->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	rUpLeg->SetShouldUpdatePhysicsVolume(true);

	lDownLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("lDownLeg"));
	lDownLeg->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("calf_l"));
	lDownLeg->SetRelativeLocation(FVector(-20.f, 0.f, 0.f));
	lDownLeg->SetRelativeRotation(NineOnZ);
	lDownLeg->InitCapsuleSize(8.f, 25.f);
	lDownLeg->CanCharacterStepUp(false);
	lDownLeg->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	lDownLeg->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	lDownLeg->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lDownLeg->SetShouldUpdatePhysicsVolume(true);

	lUpLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("lUpLeg"));
	lUpLeg->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("thigh_l"));
	lUpLeg->SetRelativeLocation(FVector(-20.f, 0.f, 0.f));
	lUpLeg->SetRelativeRotation(NineOnZ);
	lUpLeg->InitCapsuleSize(10.f, 25.f);
	lUpLeg->CanCharacterStepUp(false);
	lUpLeg->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	lUpLeg->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	lUpLeg->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lUpLeg->SetShouldUpdatePhysicsVolume(true);
	

	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	GetCharacterMovement()->bCanWalkOffLedges = false;*/
	
	enemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	animInstance = GetMesh()->GetAnimInstance();

	bPickedStrafeDirection = false;
	bBackForth = false;
	bFrozen = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetMesh()->bPauseAnims = false;
	//FActorSpawnParameters spawnInfo;
	AIController = Cast<AAIController>(GetController());
	player = Cast<ADefaultUnreal5Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GetMesh()->SetMaterial(0, frozenMaterial);

	combatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::BladeOverlapBegin);
	combatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::BladeOverlapEnd);



	if (fCurve)
	{
		FOnTimelineFloat InterpFunction{};

		InterpFunction.BindUFunction(this, FName("RotationTimeLineFloatReturn"));
		rotationTimeLine->AddInterpFloat(fCurve, InterpFunction, FName("Alpha"));
		rotationTimeLine->SetLooping(false);
		rotationTimeLine->SetIgnoreTimeDilation(true);
	}
	// set our turn rate for input
}

void AEnemy::RotationTimeLineFloatReturn(float value)
{
//	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (GetMesh()->bPauseAnims)
	{
		return;
	}

	if (!navPathPoints.IsEmpty())
	{
		//playerToAi = UKismetMathLibrary::GetDirectionUnitVector(endPoint, GetActorLocation());
		if (bBomber)
		{
			playerToAi = UKismetMathLibrary::GetDirectionUnitVector(navPathPoints[navPathIndex], GetActorLocation());
		}
		else if (bThrower)
		{
			playerToAi = UKismetMathLibrary::GetDirectionUnitVector(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), GetActorLocation());
			// UE_LOG(LogTemp, Warning, TEXT("penis"));
		}
		else
		{
			
			if (!bStrafing)
			{
			//	UE_LOG(LogTemp, Warning, TEXT("rotating"));
				playerToAi = UKismetMathLibrary::GetDirectionUnitVector(navPathPoints[navPathIndex], GetActorLocation());
			}
			else
			{
			//	UE_LOG(LogTemp, Warning, TEXT("rotating2"));
				playerToAi = UKismetMathLibrary::GetDirectionUnitVector(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), GetActorLocation());
			}
		}
		
	//	DrawDebugLine(GetWorld(), navPathPoints[navPathIndex], navPathPoints[navPathIndex] + FVector(0.0f, 0.0f, 1000.0f), FColor::Orange, false, 0.f, 0, 2.0f);
		double yawRotation = UKismetMathLibrary::MakeRotFromZ(playerToAi).Yaw;
		FRotator aimRotation = FRotator(GetActorRotation().Pitch, yawRotation + 90, GetActorRotation().Roll);
		FRotator newRotation = UKismetMathLibrary::RLerp(GetActorRotation(), aimRotation, value, true);
		SetActorRotation(newRotation);
	//S	UE_LOG(LogTemp, Warning, TEXT("%f"), value);
	
	}
	else if (endPoint != FVector(0.f))
	{
		playerToAi = UKismetMathLibrary::GetDirectionUnitVector(endPoint, GetActorLocation());
		double yawRotation = UKismetMathLibrary::MakeRotFromZ(playerToAi).Yaw;
		FRotator aimRotation = FRotator(GetActorRotation().Pitch, yawRotation + 90, GetActorRotation().Roll);
		FRotator newRotation = UKismetMathLibrary::RLerp(GetActorRotation(), aimRotation, value, true);
		SetActorRotation(newRotation);
	//	UE_LOG(LogTemp, Warning, TEXT("penis2"));
//		DrawDebugLine(GetWorld(), endPoint, endPoint + FVector(0.0f, 0.0f, 1000.0f), FColor::Red, false, 0.f, 0, 2.0f);
	}
//s	DrawDebugLine(GetWorld(), endPoint, endPoint + FVector(0.0f, 0.0f, 1000.0f), FColor::Red, false, 0.f, 0, 2.0f);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (player)
	{
		if (player->bNotDead == false)
		{
			SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			//bPlayerDead = true;
			GetMesh()->bPauseAnims = true;
		}

		NeckRotator();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::BatHitAnimation()
{
	//	SetUpEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

	if (animInstance)
	{
		//	UE_LOG(LogTemp, Warning, TEXT("parrydebug"));
		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("hitBat"));
	}
}

void AEnemy::PlayerPullAxe()
{
	GetMesh()->bPauseAnims = true;
}


void AEnemy::Unfreeze()
{
	GetMesh()->bPauseAnims = false;
}

void AEnemy::NeckRotator()
{
	FRotator lookAtRot;
//	ACharacter* toggled;

	lookAtRot = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(FName("neck_01")), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh()->GetSocketLocation(FName("neck_01")));

	if ((GetActorRotation().Yaw - lookAtRot.Yaw) < 181 && (GetActorRotation().Yaw - lookAtRot.Yaw) > -181)
	{
	
	neckRotYaw = GetActorRotation().Yaw - lookAtRot.Yaw;

	}
	else if ((GetActorRotation().Yaw - lookAtRot.Yaw) > 180)
		neckRotYaw = lookAtRot.Yaw - GetActorRotation().Yaw - 360;
	else if ((GetActorRotation().Yaw - lookAtRot.Yaw) < -180)
		neckRotYaw = lookAtRot.Yaw - GetActorRotation().Yaw + 360;

	neckRotPitch = GetActorRotation().Pitch - lookAtRot.Pitch;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), neckRotYaw);

}

float AEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *EventInstigator->GetPawn()->GetName());
	//1 = attack, 2 = throw, 3 = pullback after parry, 4 = FREEZE aka bomb, 5 = kill volume (fall), 6 = light stumble ,7 = bat, 8 = Kick 
//	UE_LOG(LogTemp, Warning, TEXT("DamageBaby3"));
	AAxe* damagerAxe;
	damagerAxe = Cast<AAxe>(DamageCauser);
	ABat* damagerBat;
	damagerBat = Cast<ABat>(DamageCauser);

	FRotator kickerRotation = EventInstigator->GetControlRotation();
	//UE_LOG(LogTemp, Warning, TEXT("Damage = %f"), Damage);

	if (Damage == 5)
	{
		Destroy();
		return 0;
	}
	else if (Damage == 6) //light stumble aka body thrown to enemy while frozen
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
		
		SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));
		BodyParry();
		return 0;
	}
	else if (Damage == 4) // Ice Bomb Hit
	{
		GetMesh()->bPauseAnims = true;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AEnemy::Unfreeze, 2.f, true);
		//UE_LOG(LogTemp, Warning, TEXT("provoke parry1"));
		return 0;
	}
	else if (Damage == 8) //Kick
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), kickedSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
		SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));

		if (bMarauder && bAxeEquipped)
		{
		////	equippedAxe->Drop();
		//	bAxeEquipped = false;
		//	equippedAxe = nullptr;
			Kicked();	
		}
		else
		{
			Parried();
		}
		
		return 0;
	}
	else if (Damage == 3) // Parry
	{

		//	damager.Drop();
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), kickedSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
		Parried();

		SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));

		if (bBatter && bAxeEquipped)
		{
			//	
			bAxeEquipped = false;
			equippedBat->Drop();
			equippedBat = nullptr;
		}
		return 0;
	}
	else //Frozen or not
	{
		if (bFrozen)
		{
		//	UE_LOG(LogTemp, Warning, TEXT("parry %f frozen"), Damage);
			if (Damage == 1 )
			{

				UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), parrySound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

				UGameplayStatics::ApplyDamage(EventInstigator->GetPawn(), 3, GetController(), this, damageTypeClass);
				//	UE_LOG(LogTemp, Warning, TEXT("provoke parry"));
				return 0;
			}
			else if (Damage == 2)
			{
				if (damagerAxe)
				{
					damagerAxe->Drop();
				//	UE_LOG(LogTemp, Warning, TEXT("provoke parry"));
				}
				else if (damagerBat)
				{
					damagerBat->Drop();
				}
					

				BodyParry();
			//	UE_LOG(LogTemp, Warning, TEXT("provoke parry2"));
				return 0;
			}
			else if (Damage == 7)
			{
				if (damagerBat->bThrown)
				{
					BodyParry();
					damagerBat->Drop();
				}
				else
				{

					UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), parrySound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

					UGameplayStatics::ApplyDamage(EventInstigator->GetPawn(), 3, GetController(), this, damageTypeClass);
				}
				//	UE_LOG(LogTemp, Warning, TEXT("provoke parry2"));
				return 0;
			}
		}
		else
		{
		//	UE_LOG(LogTemp, Warning, TEXT("parry %f not frozen"), Damage);
			if (Damage == 1 || Damage == 2)
			{
				if (animInstance)
				{
				//	UE_LOG(LogTemp, Warning, TEXT("%F"), Damage);
				//	UE_LOG(LogTemp, Warning, TEXT("DamageBaby"));
					UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
					if ((health - 3) < 1)
					{
						health -= 3;
					}
					else
					{
						health -= 3;
						BatHitBP();
					}
						
					return 0;
					
				}
			}
			else if (Damage == 7)
			{
				UE_LOG(LogTemp, Warning, TEXT("batHit"));
				
				UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), kickedSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
				if ((health - 1) < 1)
					health -= 1;
				else
				{
					health -= 1;
					BatHitBP();
					
				}
					
				SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));
				return 0;
			}

		}
		return 0;
	}
		
}

void AEnemy::DirectionMovement(TArray<FVector> pathPoints)
{
	float emptyX;
	float emptyY;
	float yawAngle;	

	float emptyX2;
	float emptyY2;
	float yawAngle2;
	navPathIndex = 0;
	FVector unitDirection;

	navPathPoints = pathPoints;
//	navPathIndex = 0; GetCapsuleComponent()->GetComponentRotation()
	if (!navPathPoints.IsEmpty())
	{
		for (FVector location : navPathPoints)
		{
			if (UKismetMathLibrary::EqualEqual_VectorVector(GetActorLocation(), navPathPoints[navPathIndex], 200))
			{
				if(navPathIndex < navPathPoints.Num() -1)
					navPathIndex++;
			}
		}

		if (bThrower && !bBomber)
		{
			unitDirection = UKismetMathLibrary::GetDirectionUnitVector(navPathPoints[navPathPoints.Num() - 1], GetActorLocation());
		}
		else
		{
			unitDirection = UKismetMathLibrary::GetDirectionUnitVector(navPathPoints[navPathIndex], GetActorLocation());
		}

		FRotator unitRot = UKismetMathLibrary::MakeRotFromY(unitDirection);
		UKismetMathLibrary::BreakRotator(unitRot, emptyX, emptyY, yawAngle);
		UKismetMathLibrary::BreakRotator(GetActorRotation(), emptyX2, emptyY2, yawAngle2);
		
//		DrawDebugLine(GetWorld(), navPathPoints[navPathIndex], navPathPoints[navPathIndex] + FVector(0.0f, 0.0f, 1000.0f), FColor::Orange, false, 0.f, 0, 2.0f);
	//	DrawDebugLine(GetWorld(), navPathPoints[0], navPathPoints[0] + FVector(0.0f, 0.0f, 1000.0f), FColor::Red, false, 0.f, 0, 2.0f);
		
		if (navPathPoints.Num() != 0)
		{

			if ((yawAngle - yawAngle2) < 181 && (yawAngle - yawAngle2) > -181)
			{
					direction = yawAngle - yawAngle2;
				//	UE_LOG(LogTemp, Warning, TEXT("Direction  - : %f"), direction);
			//		UE_LOG(LogTemp, Warning, TEXT(" "));
			}
			else if ((yawAngle - yawAngle2) > 180)
			{
					direction = yawAngle - yawAngle2 - 360	;
					
			//		UE_LOG(LogTemp, Warning, TEXT("higher"));
			}
			else if ((yawAngle - yawAngle2) < -180)
			{
				direction = yawAngle - yawAngle2 + 360;

			}

		}

	}
}

void AEnemy::Strafe()
{
	FHitResult hit;
	FHitResult hit2;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray2;
	ObjectTypesArray.Reserve(1);
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypesArray2.Reserve(1);
	ObjectTypesArray2.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor*> ignoredActors;

	FRotator math;
	if (bThrower)
	{
		
		math =UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
		
		endPoint = UKismetMathLibrary::GetForwardVector(math) * -100 + GetActorLocation();
	//	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), endPoint, endPoint, 30.f, ObjectTypesArray, false, ignoredActors, EDrawDebugTrace::ForDuration, hit, true);

		UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), endPoint + GetActorUpVector() * -100, endPoint + GetActorUpVector() * -100, 30.f, ObjectTypesArray, false, ignoredActors, EDrawDebugTrace::None, hit, true);
		if (hit.bBlockingHit)
		{
			//If no walk to walk to then strafe
			UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), endPoint, endPoint, 50.f, ObjectTypesArray2, false, ignoredActors, EDrawDebugTrace::None, hit2, true);
			if (hit2.bBlockingHit)
			{
			//	UE_LOG(LogTemp, Warning, TEXT("Got Wall"));
				endPoint = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			}
		}
		else
		{
		//	UE_LOG(LogTemp, Warning, TEXT("Got no Floor"));
			endPoint = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
		}
	}
	else
	{
		if (bPickedStrafeDirection)
		{
	
			if (bBackForth)
			{


			//	UE_LOG(LogTemp, Warning, TEXT("Forward"));
				endPoint = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			}
			else
			{
				endPoint = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			}
		}
		else
		{
		
			bPickedStrafeDirection = true;
			bLeftRight2 = bLeftRight;
			bBackForth = bLeftRight;
		}
	}
}
	
void AEnemy::RotateTowardsPlayer()
{
	if (!GetMesh()->bPauseAnims || enemyMovementStatus != EEnemyMovementStatus::EMS_Dead)
	{
		rotationTimeLine->PlayFromStart();
	}
}

void AEnemy::SetupKickCollision(bool activate)
{

		if (activate)
		{
			combatCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
		{
			combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

}

void AEnemy::BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		combatInstigator = this->GetInstigatorController();
		UGameplayStatics::ApplyDamage(OtherActor, 3, combatInstigator, this, damageTypeClass);

		combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void AEnemy::BladeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
