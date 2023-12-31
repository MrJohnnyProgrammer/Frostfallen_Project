// Fill out your copyright notice in the Description page of Project Settings.


#include "Bear.h"
#include "Axe.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


void ABear::BeginPlay()
{
	Super::BeginPlay();

	animInstance = GetMesh()->GetAnimInstance();
	health = 6;
	bear = true;


	bRotate = false;
}
// Called every frame

void ABear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABear::Turn(bool bDirection)
{
	if (animInstance)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		if (bDirection)
		{
		//	UE_LOG(LogTemp, Warning, TEXT("left"));
			animInstance->Montage_Play(attackMontage, 1.f);
			animInstance->Montage_JumpToSection(FName("turnLeft"));
		}
		else
		{
		//	UE_LOG(LogTemp, Warning, TEXT("right"));
			animInstance->Montage_Play(attackMontage, 1.f);
			animInstance->Montage_JumpToSection(FName("turnRight"));
		}
	}

}
void ABear::DistanceAOE() 
{
	if (animInstance)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), screamSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("AOE"));

	}
}

void ABear::Transition() 
{
	if (animInstance)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), screamSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("headShot"));
	}
}

void ABear::GotHit() 
{
	if (animInstance)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("hit"));

	}
}

void ABear::HeadShot() 
{
	if (animInstance)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("headShot"));

	}
}

void ABear::Stomp() 
{
	if (animInstance)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("stomp"));

	}
}

void ABear::BackAOE() 
{
	if (animInstance)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("AOE"));

	}
}

void ABear::Charge() 
{
	if (animInstance)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), screamSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		animInstance->Montage_Play(attackMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("charge"));
	}
}

void ABear::CloseRange() 
{
	if (animInstance)
	{
		UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), swingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		if (!bPhase2)
		{
			animInstance->Montage_Play(attackMontage, 1.f);
			animInstance->Montage_JumpToSection(FName("scratch"));
		}
		else
		{
			animInstance->Montage_Play(attackMontage, 1.f);
			animInstance->Montage_JumpToSection(FName("bite"));
		}
	}
}
