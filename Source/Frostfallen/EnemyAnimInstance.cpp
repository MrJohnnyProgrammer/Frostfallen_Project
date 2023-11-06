// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DefaultUnreal5Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"
#include "Materials/MaterialInterface.h"
#include "Engine/DecalActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy.h" 
#include "NPC.h"


void UEnemyAnimInstance::NativeInitializeAnimation()
{
	ikInterpSpeed = 20;
	feetDisplacement = 0.f;

	//decalStep = CreateDefaultSubobject<ADecalActor>(TEXT("decal"));

	LRot = FRotator(0.f);
	RRot = FRotator(0.f);
	RIK = 0.f;
	LIK = 0.f;
	aoPitch = 0.f;

	if (pawn == nullptr)
	{
		pawn = TryGetPawnOwner();
		if (pawn) {
			enemy = Cast<AEnemy>(pawn);
		}
	}
}



void UEnemyAnimInstance::UpdateAnimationProperties()
{

	if (pawn == nullptr)
	{
		pawn = TryGetPawnOwner();
	}
	else
	{
		FVector speed = pawn->GetVelocity();
		FVector lateralSpeed = FVector(speed.X, speed.Y, 0.f);

		movementSpeed = lateralSpeed.Size();

		bIsinAir = pawn->GetMovementComponent()->IsFalling();
		
	}
}


void UEnemyAnimInstance::Displacement(FName socketName, float& outDistance, bool& outBool) {

	if (enemy)
	{
		FVector meshLocation = enemy->GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 98.f);
		FVector socketLocation = enemy->GetMesh()->GetSocketLocation(socketName);

		FHitResult hit;
		FCollisionQueryParams params;
		FCollisionResponseParams rParams;
		params.AddIgnoredActor(enemy);
	//	params.AddIgnoredActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		FVector baseVector = FVector(socketLocation.X, socketLocation.Y, meshLocation.Z);

		//DrawDebugLine(GetWorld(), baseVector, baseVector - FVector(0.0f, 0.0f, 300.0f), FColor::Orange, false, 0.f, 0, 2.0f);

		if (GetWorld()->LineTraceSingleByChannel(hit, baseVector, baseVector - FVector(0.0f, 0.0f, 151.f), ECollisionChannel::ECC_Camera, params, rParams))
		{
			outDistance = hit.Distance;
			//	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), hit.Distance);
			outBool = hit.bBlockingHit;
		}

		else
		{
			outDistance = 0.f;
			//	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), hit.Distance);
			outBool = false;
		}
	}
}

void UEnemyAnimInstance::LineTrace(FName socketName, float& outDistance, FVector& outNormal)
{

	if (enemy) {
		FVector socketLocation = enemy->GetMesh()->GetSocketLocation(socketName);
		FVector rootLocation = enemy->GetMesh()->GetSocketLocation(FName("root"));
		FHitResult hit;

		FVector baseVector = FVector(socketLocation.X, socketLocation.Y, rootLocation.Z);

		FCollisionQueryParams params;
		FCollisionResponseParams rParams;
		params.AddIgnoredActor(enemy);
	//	params.AddIgnoredActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (GetWorld()->LineTraceSingleByChannel(hit, baseVector + FVector(0.f, 0.f, 100.f), baseVector + FVector(0.0f, 0.0f, -100.0f),
			ECollisionChannel::ECC_Camera, params, rParams))
		{
	//	DrawDebugLine(GetWorld(), baseVector + FVector(0.f, 0.f, 100.f), baseVector + FVector(0.0f, 0.0f, -100.0f), FColor::Red, false, 0.f, 0, 2.0f);
			outDistance = hit.Distance;
			outNormal = hit.Normal;

		}
		else {
			outDistance = 999.f;
			outNormal = FVector(0.f);
		}
	}
}

void  UEnemyAnimInstance::FootPlacement()
{
	float footRDistance = 0;
	float footLDistance = 0;
	float footRIkMath = 0;
	float footLIkMath = 0;
	float rotatorX = 0;
	float rotatorY = 0;

	bool footRBool = false;
	bool footLBool = false;


	FVector IKRNormal = FVector(0.f);
	FVector IKLNormal = FVector(0.f);

	FRotator TargetRotation = FRotator(0.f);

	float feetInterpTarget = 0;


	if (!bIsinAir)
	{

		Displacement(FName("foot_r"), footRDistance, footRBool);
		Displacement(FName("foot_l"), footLDistance, footLBool);

		if (footLBool || footRBool) {

			//UE_LOG(LogTemp, Warning, TEXT("footRDistance: %f"), footRDistance);
			//UE_LOG(LogTemp, Warning, TEXT("footLDistance: %f"), footLDistance);

			feetInterpTarget = (UKismetMathLibrary::SelectFloat(footRDistance, footLDistance, footRDistance >= footLDistance) - 98) * -1;

			feetDisplacement = UKismetMathLibrary::FInterpTo(feetDisplacement, feetInterpTarget - 5, GetDeltaSeconds(), 3.5);

			//UE_LOG(LogTemp, Warning, TEXT("Feet Displacement: %f"), feetDisplacement);

			//GETTING ROTATION AND HEIGHT FOR R LEG
			LineTrace(FName("foot_r"), footRIkMath, IKRNormal);
			//UE_LOG(LogTemp, Warning, TEXT("footRIkMath: %f"), footRIkMath);
			footRIkMath = (footRIkMath - 105) / -45;
			rotatorX = UKismetMathLibrary::Atan2(IKRNormal.Y, IKRNormal.Z);
			rotatorY = UKismetMathLibrary::Atan2(IKRNormal.X, IKRNormal.Z) * -1;
			TargetRotation = FRotator(rotatorY, 0.0f, rotatorX);
			RRot = UKismetMathLibrary::RInterpTo(RRot, TargetRotation * 100, GetDeltaSeconds(), ikInterpSpeed);
			//UE_LOG(LogTemp, Warning, TEXT("RRot: X=%f, Y=%f, Z=%f"), RRot.Roll, RRot.Pitch, RRot.Yaw);
			RIK = UKismetMathLibrary::FInterpTo(RIK, footRIkMath, GetDeltaSeconds(), ikInterpSpeed);



			LineTrace(FName("foot_l"), footLIkMath, IKLNormal);
			//UE_LOG(LogTemp, Warning, TEXT("footLIkMath: %f"), footLIkMath);
			footLIkMath = (footLIkMath - 105) / -45;
			rotatorX = UKismetMathLibrary::Atan2(IKLNormal.Y, IKLNormal.Z);
			rotatorY = UKismetMathLibrary::Atan2(IKLNormal.X, IKLNormal.Z) * -1;
			TargetRotation = FRotator(rotatorY, 0.0f, rotatorX);
			LRot = UKismetMathLibrary::RInterpTo(LRot, TargetRotation * 100, GetDeltaSeconds(), ikInterpSpeed);
			//UE_LOG(LogTemp, Warning, TEXT("LRot: X=%f, Y=%f, Z=%f"), LRot.Roll, LRot.Pitch, LRot.Yaw);
			LIK = UKismetMathLibrary::FInterpTo(LIK, footLIkMath, GetDeltaSeconds(), ikInterpSpeed);
		}
	}
}


void UEnemyAnimInstance::FootFX(bool leftFoot)
{
	if (enemy)
	{
		UPhysicalMaterial* floor;
		FActorSpawnParameters spawnInfo;
		float yawOffset = 0;
		FVector socketLocation;
		FRotator socketRotation;
		FHitResult hit;
		FCollisionQueryParams params;
		FCollisionResponseParams rParams;

		quietAttenuation = enemy->quietAttenuation;
		concurrency = enemy->concurrency;

		params.AddIgnoredActor(enemy);
		params.AddIgnoredActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		params.bReturnPhysicalMaterial = true;

		if (leftFoot)
		{
			socketLocation = enemy->GetMesh()->GetSocketLocation("foot_l_Socket");
			yawOffset = 80;
			socketRotation = enemy->GetMesh()->GetSocketRotation("foot_l_Socket");
		}
		else
		{
			socketRotation = enemy->GetMesh()->GetSocketRotation("foot_r_Socket");
			socketLocation = enemy->GetMesh()->GetSocketLocation("foot_r_Socket");
			//yawOffset = 50;
		}

		

		if (GetWorld()->LineTraceSingleByChannel(hit, socketLocation, socketLocation + FVector(0.0f, 0.0f, -150.0f) ,
			ECollisionChannel::ECC_Visibility, params, rParams))
		{
			floor = hit.PhysMaterial.Get();
			EPhysicalSurface surface = floor->SurfaceType.GetValue();
			FVector rotation = hit.ImpactNormal * -1;

			decalStep = GetWorld()->SpawnActor<ADecalActor>(hit.Location, FRotator(rotation.Y, socketRotation.Yaw + yawOffset, rotation.X), spawnInfo);



			switch (surface)
			{
			case SurfaceType1:
				//	UE_LOG(LogTemp, Warning, TEXT("inside"));
				decalStep->SetDecalMaterial(enemy->insideStepDecal);
				stepParticle = enemy->insideStepParticle;
				stepSound = enemy->insideStepSound;
				break;

			case SurfaceType2:
				//	UE_LOG(LogTemp, Warning, TEXT("snow"));
				decalStep->SetDecalMaterial(enemy->snowStepDecal);
				stepParticle = enemy->snowStepParticle;
				stepSound = enemy->snowStepSound;
				break;

			case SurfaceType_Default:
				//	UE_LOG(LogTemp, Warning, TEXT("Default"));
				decalStep->SetDecalMaterial(enemy->insideStepDecal);
				stepParticle = enemy->insideStepParticle;
				stepSound = enemy->insideStepSound;
				break;
			}

			if (decalStep && stepParticle && stepSound)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, stepParticle, socketLocation, FRotator(0.f));
				UGameplayStatics::PlaySoundAtLocation(this, stepSound, socketLocation, 0.5f, 1.f, 0.f, quietAttenuation, concurrency);

				decalStep->SetLifeSpan(20);
				//UE_LOG(LogTemp, Warning, TEXT(" %s"), *stepSound->GetFName().ToString());
				if (leftFoot)
				{

					decalStep->SetActorScale3D(FVector(0.05f, -0.03f, 0.05f));
				}
				else
				{
					decalStep->SetActorScale3D(FVector(0.05f, 0.03f, 0.05f));
				}
			}
		}
	}
}
