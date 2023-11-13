// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCS.h"
#include "DefaultUnreal5Character.h"
#include "Axe.h"
#include "Bat.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
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

ANPCS::ANPCS()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	interactionRange = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Range"));
	interactionRange->SetSphereRadius(500.f);
	interactionRange->SetupAttachment(GetCapsuleComponent());
	interactionRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	interactionRange->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	interactionRange->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	interactionRange->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	bPlayerInRange = false;
}

void ANPCS::BeginPlay()
{
	Super::BeginPlay();

	interactionRange->OnComponentBeginOverlap.AddDynamic(this, &ANPCS::DialogueOverlapBegin);
	interactionRange->OnComponentEndOverlap.AddDynamic(this, &ANPCS::DialogueOverlapEnd);

	player = Cast<ADefaultUnreal5Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ANPCS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bPlayerInRange)
	{
		NeckRotator();
	}
}


void ANPCS::DialogueTrigger(bool next, ADefaultUnreal5Character* getPlayer)
{
	
	if (next)
	{
		
	}
	else
	{
		player = getPlayer;
		
	}
}

void ANPCS::DialogueOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bPlayerInRange = true;
}


void ANPCS::DialogueOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bPlayerInRange = false;

}

void ANPCS::NeckRotator()
{
	FRotator lookAtRot;
	//	ACharacter* toggled;

	lookAtRot = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(FName("neck_01")), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh()->GetSocketLocation(FName("neck_01")));

	if ((GetActorRotation().Yaw - lookAtRot.Yaw) < 181 && (GetActorRotation().Yaw - lookAtRot.Yaw) > -181)
	{
		neckRotYaw = GetActorRotation().Yaw - lookAtRot.Yaw;
	}
	else if ((GetActorRotation().Yaw - lookAtRot.Yaw) > 180)
		neckRotYaw = GetActorRotation().Yaw - lookAtRot.Yaw - 360;
	else if ((GetActorRotation().Yaw - lookAtRot.Yaw) < -180)
		neckRotYaw = GetActorRotation().Yaw - lookAtRot.Yaw + 360;


	neckRotPitch = GetActorRotation().Pitch - lookAtRot.Pitch;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), neckRotYaw);

}

float ANPCS::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *EventInstigator->GetPawn()->GetName());
	//1 = attack, 2 = throw, 3 = pullback after parry and also kick, 4 = FREEZE
//	UE_LOG(LogTemp, Warning, TEXT("DamageBaby3"));
	AAxe* damagerAxe;
	damagerAxe = Cast<AAxe>(DamageCauser);
	ABat* damagerBat;
	damagerBat = Cast<ABat>(DamageCauser);
	return 0;
/*	if (Damage == 4)
	{
		GetMesh()->bPauseAnims = true;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AEnemy::Unfreeze, 2.f, true);
		UE_LOG(LogTemp, Warning, TEXT("provoke parry1"));
		return 0;
	}
	else
	{
		if (bFrozen)
		{
			if (Damage == 1)
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
					//	damagerAxe->Drop();
					UE_LOG(LogTemp, Warning, TEXT("provoke parry"));
				}

				else if (damagerBat)
					damagerBat->Drop();

				BodyParry();
				//	UE_LOG(LogTemp, Warning, TEXT("provoke parry2"));
				return 0;
			}
			else if (Damage == 3)
			{
				//	damager.Drop();
				UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), kickedSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
				Parried();
				FRotator kickerRotation = DamageCauser->GetActorRotation();
				SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));
				//	UE_LOG(LogTemp, Warning, TEXT("Parried Called"));
				return 0;
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("parry %f not frozen"), Damage);
			if (Damage == 1 || Damage == 2)
			{
				if (animInstance)
				{
					//	UE_LOG(LogTemp, Warning, TEXT("%F"), Damage);
					//	UE_LOG(LogTemp, Warning, TEXT("DamageBaby"));
					UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
					health -= 2;
					return 0;

				}
			}
			else if (Damage == 3)
			{
				//UE_LOG(LogTemp, Warning, TEXT("KickDamage"));
				UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), kickedSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

				Parried();
				FRotator kickerRotation = DamageCauser->GetActorRotation();
				SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));
				return 0;
			}
		}
		return 0;
	}
	*/
}

void ANPCS::DirectionMovement(TArray<FVector> pathPoints)
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
				if (navPathIndex < navPathPoints.Num() - 1)
					navPathIndex++;
			}
		}

		FRotator unitRot = UKismetMathLibrary::MakeRotFromY(unitDirection);
		UKismetMathLibrary::BreakRotator(unitRot, emptyX, emptyY, yawAngle);
		UKismetMathLibrary::BreakRotator(GetActorRotation(), emptyX2, emptyY2, yawAngle2);

		//	DrawDebugLine(GetWorld(), navPathPoints[navPathIndex], navPathPoints[navPathIndex] + FVector(0.0f, 0.0f, 1000.0f), FColor::Orange, false, 0.f, 0, 2.0f);
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
				direction = yawAngle - yawAngle2 - 360;

				//		UE_LOG(LogTemp, Warning, TEXT("higher"));
			}
			else if ((yawAngle - yawAngle2) < -180)
			{
				direction = yawAngle - yawAngle2 + 360;

				//	UE_LOG(LogTemp, Warning, TEXT("lower"));
			}

		}

	}
}
void ANPCS::EndDialogueShort()
{
}

