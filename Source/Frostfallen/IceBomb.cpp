// Fill out your copyright notice in the Description page of Project Settings.


#include "IceBomb.h"
#include "Enemy.h" 
#include "Components/StaticMeshComponent.h"
#include "DefaultUnreal5Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Sound/SoundAttenuation.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundConcurrency.h"


// Sets default values
AIceBomb::AIceBomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	staticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	staticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	staticMesh->SetSimulatePhysics(true);
	staticMesh->SetLinearDamping(2.f);
	staticMesh->SetWorldScale3D(FVector(0.1f));
	staticMesh->SetAngularDamping(1.f);

	SetRootComponent(staticMesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));

	ProjectileMovementComponent->SetUpdatedComponent(staticMesh);
	ProjectileMovementComponent->bAutoActivate = false;
	ProjectileMovementComponent->bSimulationEnabled = false;
	ProjectileMovementComponent->Deactivate();
	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = 6000.0f;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.1f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.2f;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
	ProjectileMovementComponent->bSweepCollision = true;


}

// Called when the game starts or when spawned
void AIceBomb::BeginPlay()
{
	Super::BeginPlay();
	
	staticMesh->OnComponentHit.AddDynamic(this, &AIceBomb::OnBombHit);

}

// Called every frame
void AIceBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIceBomb::OnBombHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != bombHolder)
	{
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AOEParticles, Hit.ImpactPoint,GetActorRotation(), FVector(1.f), true, true, ENCPoolMethod::None,true);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AOEParticles, Hit.ImpactPoint);

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), boomSound, Hit.ImpactPoint, GetActorRotation(), 1.f, 1.f, 0.f, LoudAttenuation, concurrency, false);

		TArray<FHitResult> OutResults;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		ObjectTypesArray.Reserve(1);
		ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle));
		TArray<AActor*> ignoredActors;

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint, 150.f, ObjectTypesArray, false, ignoredActors, EDrawDebugTrace::None, OutResults, true);
		//UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint, 150.f, ObjectTypesArray, false, ignoredActors, EDrawDebugTrace::ForDuration, hit, true);

		//UE_LOG(LogTemp, Warning, TEXT("%f"), OutResults.Num());
		for (int i = 0; i < OutResults.Num(); i++) {
			if (OutResults[i].bBlockingHit)
			{
			//	UE_LOG(LogTemp, Warning, TEXT("dick"));
				UGameplayStatics::ApplyDamage(OutResults[i].GetActor(), 4, OutResults[i].GetActor()->GetInstigatorController(), this, damageTypeClass);
			}
			//UGameplayStatics::ApplyDamage(OutResults[i].GetActor(), 4, OutResults[i].GetActor()->GetInstigatorController() , this, damageTypeClass);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *OutResults[i].GetActor()->GetName());
		}

		Destroy();
	}
}


void AIceBomb::Thrown(const FVector& direction, FRotator throwRot, bool isAi)
{
	
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	ProjectileMovementComponent->Velocity = direction * 2;

	float throwYaw = direction.Z / ProjectileMovementComponent->Velocity.Size();
	float throwPitch = direction.Y / ProjectileMovementComponent->Velocity.Size();
	float throwRoll = direction.X / ProjectileMovementComponent->Velocity.Size();

	throwYaw *= 3000.f;
	throwPitch *= 3000.f;
	throwRoll *= 3000.f;

//	SetActorRotation(FRotator(GetActorRotation().Pitch, throwRot.Yaw, GetActorRotation().Roll));
	ProjectileMovementComponent->Velocity = FVector(throwRoll, throwPitch, throwYaw);
	//UE_LOG(LogTemp, Warning, TEXT("throw player: %f %f %f"), throwRoll, throwPitch, throwYaw);
	ProjectileMovementComponent->bSimulationEnabled = true;
	ProjectileMovementComponent->Activate();
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AIceBomb::AllowCollision, 0.1f, false);

}

void AIceBomb::AllowCollision()
{
	staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


void AIceBomb::EquipBomb(ACharacter* player)
{
	if (player)
	{
		bombHolder = player;
		ProjectileMovementComponent->Deactivate();
		staticMesh->SetSimulatePhysics(false);
		staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileMovementComponent->bSimulationEnabled = false;

		const USkeletalMeshSocket* rightHandSocket = player->GetMesh()->GetSocketByName("RightHandSo");

		if (rightHandSocket)
		{
			rightHandSocket->AttachActor(this, player->GetMesh());
		}

	}
}
