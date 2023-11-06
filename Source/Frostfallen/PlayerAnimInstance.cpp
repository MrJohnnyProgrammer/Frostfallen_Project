// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
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
#include "Axe.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	ikInterpSpeed = 20;
	feetDisplacement = 0.f;

	//decalStep = CreateDefaultSubobject<ADecalActor>(TEXT("decal"));

	 LRot = FRotator(0.f);
	 RRot = FRotator(0.f);
	 RIK = 0.f;
	 LIK = 0.f;
	 aoPitch = 0.f;
	 bDodging = false;
	 bLocked = false;

	if (pawn == nullptr)
	{
		pawn = TryGetPawnOwner();
		if (pawn) {
			player = Cast<ADefaultUnreal5Character>(pawn);
		}
	}
}



void UPlayerAnimInstance::UpdateAnimationProperties()
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
//		UE_LOG(LogTemp, Warning, TEXT("speed: %f"), movementSpeed);
	
		bIsinAir = pawn->GetMovementComponent()->IsFalling();

		if (player != nullptr) {
			
			direction = player->GetWalkingDirectionAngle();
		//	UE_LOG(LogTemp, Warning, TEXT("footR Yaw: %f"), player->GetMesh()->GetSocketRotation("foot_r_Socket").Yaw);
		//	UE_LOG(LogTemp, Warning, TEXT("footL Yaw: %f"), player->GetMesh()->GetSocketRotation("foot_l_Socket").Yaw);

		}
		else
		{
			player = Cast<ADefaultUnreal5Character>(pawn);
		}
	}
}

void UPlayerAnimInstance::setAOPitch()
{
	FRotator aimAngle;
	FRotator delta;
	FRotator pitch = FRotator(aoPitch, 0.f, 0.f);

	if (player)
	{
		delta = UKismetMathLibrary::NormalizedDeltaRotator(player->GetViewRotation(), player->GetActorRotation());
	}
	else
	{
		player = Cast<ADefaultUnreal5Character>(pawn);
	}
	aimAngle  = UKismetMathLibrary::RInterpTo(pitch, delta, GetDeltaSeconds(), 15);
	aoPitch = UKismetMathLibrary::ClampAngle(aimAngle.Pitch, -40.f, 40.f);
}

void UPlayerAnimInstance::Displacement(FName socketName, float &outDistance, bool &outBool ) {

	if (player )
	{
		FVector meshLocation = player->GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 98.f);
		FVector socketLocation = player->GetMesh()->GetSocketLocation(socketName);

		FHitResult hit;
		FCollisionQueryParams params;
		FCollisionResponseParams rParams;
		params.AddIgnoredActor(player);

		FVector baseVector = FVector(socketLocation.X, socketLocation.Y, meshLocation.Z);

//		DrawDebugLine(GetWorld(), baseVector, baseVector - FVector(0.0f, 0.0f, 300.0f), FColor::Orange, false, 0.f, 0, 2.0f);

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

void UPlayerAnimInstance::LineTrace(FName socketName, float& outDistance, FVector& outNormal) 
{

	if (player ) {
		FVector socketLocation = player->GetMesh()->GetSocketLocation(socketName);
		FVector rootLocation = player->GetMesh()->GetSocketLocation(FName("root"));
		FHitResult hit;

		FVector baseVector = FVector(socketLocation.X, socketLocation.Y, rootLocation.Z);

		FCollisionQueryParams params;
		FCollisionResponseParams rParams;
		params.AddIgnoredActor(player);

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

void  UPlayerAnimInstance::FootPlacement()
{
	float footRDistance= 0;
	float footLDistance=0;
	float footRIkMath = 0;
	float footLIkMath = 0;
	float rotatorX = 0;
	float rotatorY = 0;

	bool footRBool = false ;
	bool footLBool = false;


	FVector IKRNormal = FVector(0.f);
	FVector IKLNormal = FVector(0.f);

	FRotator TargetRotation = FRotator(0.f);
	
	float feetInterpTarget = 0 ;

	if (!player)
		return;
	
	if (!bIsinAir && player->bNotDead)
	{

		Displacement(FName("foot_r"), footRDistance, footRBool);
		Displacement(FName("foot_l"), footLDistance, footLBool);

		if (footLBool || footRBool) {

			//UE_LOG(LogTemp, Warning, TEXT("footRDistance: %f"), footRDistance);
			//UE_LOG(LogTemp, Warning, TEXT("footLDistance: %f"), footLDistance);
			
			feetInterpTarget = (UKismetMathLibrary::SelectFloat(footRDistance, footLDistance, footRDistance >= footLDistance) - 98 ) * -1 ;

			feetDisplacement = UKismetMathLibrary::FInterpTo(feetDisplacement, feetInterpTarget - 5, GetDeltaSeconds(), 3.5);
			
			//UE_LOG(LogTemp, Warning, TEXT("Feet Displacement: %f"), feetDisplacement);

			//GETTING ROTATION AND HEIGHT FOR R LEG
			LineTrace(FName("foot_r"), footRIkMath, IKRNormal);	
			//UE_LOG(LogTemp, Warning, TEXT("footRIkMath: %f"), footRIkMath);
			footRIkMath = (footRIkMath - 105) / -45;
			rotatorX = UKismetMathLibrary::Atan2(IKRNormal.Y, IKRNormal.Z);
			rotatorY = UKismetMathLibrary::Atan2(IKRNormal.X, IKRNormal.Z) * -1;
		
			if (rotatorX > 0.1)
			{
				rotatorX = 0.1;
			}
			else if (rotatorX < -0.10)
			{
				rotatorX = -0.1;
			}

			if (rotatorY > 0.20)
			{
				rotatorY = 0.20;
			}
			else if (rotatorY < -0.20)
			{
				rotatorY = -0.20;
			}
			//UE_LOG(LogTemp, Warning, TEXT("rotX R: %f"), rotatorX);
			//UE_LOG(LogTemp, Warning, TEXT("rotY R: %f"), rotatorY);
			TargetRotation = FRotator(rotatorY, 0.0f, rotatorX);
			RRot = UKismetMathLibrary::RInterpTo(RRot, TargetRotation * 100, GetDeltaSeconds(), ikInterpSpeed);
		//	UE_LOG(LogTemp, Warning, TEXT("RRot: X=%f, Y=%f, Z=%f"), RRot.Roll, RRot.Pitch, RRot.Yaw);
			RIK = UKismetMathLibrary::FInterpTo(RIK, footRIkMath, GetDeltaSeconds(), ikInterpSpeed);
			
			

			LineTrace(FName("foot_l"), footLIkMath, IKLNormal);
			//UE_LOG(LogTemp, Warning, TEXT("footLIkMath: %f"), footLIkMath);
			footLIkMath = (footLIkMath - 105) / -45;
			rotatorX = UKismetMathLibrary::Atan2(IKLNormal.Y, IKLNormal.Z);
			rotatorY = UKismetMathLibrary::Atan2(IKLNormal.X, IKLNormal.Z) * -1;
			
			if (rotatorX > 0.2)
			{
				rotatorX = 0.2;
			}
			else if (rotatorX < -0.20)
			{
				rotatorX = -0.2;
			}

			if (rotatorY > 0.20)
			{
				rotatorY = 0.20;
			}
			else if (rotatorY < -0.20)
			{
				rotatorY = -0.20;
			}
		//	UE_LOG(LogTemp, Warning, TEXT("rotX L: %f"), rotatorX);
		//	UE_LOG(LogTemp, Warning, TEXT("rotY L: %f"), rotatorY);
			TargetRotation = FRotator(rotatorY, 0.0f, rotatorX);
			LRot = UKismetMathLibrary::RInterpTo(LRot, TargetRotation * 100, GetDeltaSeconds(), ikInterpSpeed);
		//	UE_LOG(LogTemp, Warning, TEXT("LRot: X=%f, Y=%f, Z=%f"), LRot.Roll, LRot.Pitch, LRot.Yaw);
			LIK = UKismetMathLibrary::FInterpTo(LIK, footLIkMath, GetDeltaSeconds(), ikInterpSpeed);
			
		}	
	}
}


void UPlayerAnimInstance::FootFX(bool leftFoot)
{
	if (player)
	{
		UPhysicalMaterial* floor;
		FActorSpawnParameters spawnInfo;
		float yawOffset = 0;
		FVector socketLocation;
		FRotator socketRotation;
		FHitResult hit;
		FCollisionQueryParams params;
		FCollisionResponseParams rParams;
		params.AddIgnoredActor(player);
		params.bReturnPhysicalMaterial = true;


		quietAttenuation = player->quietAttenuation;
		concurrency = player->concurrency;


		if (leftFoot)
		{

			socketRotation = player->GetMesh()->GetSocketRotation("foot_l_Socket");
			socketLocation = player->GetMesh()->GetSocketLocation("foot_l_Socket");	

		}
		else 
		{
			socketRotation = player->GetMesh()->GetSocketRotation("foot_r_Socket");
			socketLocation = player->GetMesh()->GetSocketLocation("foot_r_Socket");

		}


		if (GetWorld()->LineTraceSingleByChannel(hit, socketLocation, socketLocation + FVector(0.f, 0.f, -150.0f),
			ECollisionChannel::ECC_Visibility, params, rParams)) 
		{
			floor = hit.PhysMaterial.Get();
			EPhysicalSurface surface = floor->SurfaceType.GetValue();
			FVector rotation = hit.ImpactNormal * -1;

			decalStep = GetWorld()->SpawnActor<ADecalActor>(hit.Location, FRotator(rotation.Y, socketRotation.Yaw, rotation.X), spawnInfo);
		//	UE_LOG(LogTemp, Warning, TEXT("Yaw+offset: %f"), socketRotation.Yaw + yawOffset);
			
			switch (surface)
			{
			case SurfaceType1:
			//	UE_LOG(LogTemp, Warning, TEXT("inside"));
			//	decalStep->SetDecalMaterial(player->insideStepDecal);
				decalStep->SetDecalMaterial(player->insideStepDecal);
				stepParticle = player->insideStepParticle;
				stepSound = player->insideStepSound;
				break;

			case SurfaceType2:
			//	UE_LOG(LogTemp, Warning, TEXT("snow"));
				decalStep->SetDecalMaterial(player->snowStepDecal);
				stepParticle = player->snowStepParticle;
				stepSound = player->snowStepSound;
				break;

			case SurfaceType_Default:
			//	UE_LOG(LogTemp, Warning, TEXT("Default"));
				decalStep->SetDecalMaterial(player->snowStepDecal);
				stepParticle = player->snowStepParticle;
				stepSound = player->snowStepSound;
				break;
			}
			
			if (decalStep && stepParticle && stepSound)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, stepParticle, socketLocation, FRotator(0.f));

				UGameplayStatics::PlaySoundAtLocation(this, stepSound, socketLocation, 0.4f, 1.f, 0.f, quietAttenuation, concurrency);
				
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
