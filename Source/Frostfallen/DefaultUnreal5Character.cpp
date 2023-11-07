// Copyright Epic Games, Inc. All Rights Reserved.

#include "DefaultUnreal5Character.h"
#include "GameFramework/Controller.h"
#include "MainPlayerController.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Axe.h"
#include "Bat.h"
#include "IceBomb.h"
#include "Enemy.h"
#include "PlayerAnimInstance.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Actor.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "EnemySpawnPoint.h"
#include "StaticPickUp.h"
#include "StaticPickUpSpawnPoint.h"
#include "NPCS.h"
#include "Templates/SubclassOf.h"


//////////////////////////////////////////////////////////////////////////
// ADefaultUnreal5Character

ADefaultUnreal5Character::ADefaultUnreal5Character()
{

	PrimaryActorTick.bCanEverTick = true;

	
	aimTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("Aim Timeline"));

	toggleTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("Toggle Timeline"));

	aimTimeLineReverse = CreateDefaultSubobject<UTimelineComponent>(TEXT("Aim Timeline Reverse"));

	toggleTimeLineReverse = CreateDefaultSubobject<UTimelineComponent>(TEXT("Toggle Timeline Reverse"));

	sprintTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("Sprint Camera Timeline"));

	// Set collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	
	GetCapsuleComponent()->bHiddenInGame = false;
	
	
	GetMesh()->SetCollisionProfileName(FName("Pawn"));
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);

//	bombsStaticMesh.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb1")));
	bombsStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb2"));
	bombsStaticMesh->SetupAttachment(GetMesh());
	bombsStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("pelvis"));
	bombsStaticMesh->SetVisibility(false);
	bombsStaticMesh->SetRelativeLocation(FVector(10.f,-17.f,10.f));
//	bombsStaticMesh.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb3")));
//	bombs->SetupAttachment(GetMesh());
	/*for (auto bomb : bombsStaticMesh)
	{
		bomb->SetupAttachment(GetMesh());
		bomb->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bomb->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("pelvis"));
		//bomb->SetVisibleFlag(false);
		//bomb->SetVisibility(false, true);
		bomb->SetRelativeLocation(FVector(0.f));
	}*/

	headSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Mesh"));
	headSkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	headSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	headSkeletalMesh->SetupAttachment(GetMesh());

	clothingSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Clothing Mesh"));
	clothingSkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	clothingSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	clothingSkeletalMesh->SetupAttachment(GetMesh());

	FRotator NineOnZ = FRotator(90.f, 0.f, 0.f);

	bMovingForward = false;
	bDodge = false;
	bToggledOn = false;
	bMovingRight = false;
	bAiming = false;
	toggledOnActor = nullptr;
	closestAIToTarget = nullptr;
	bNotDead = true;
	bAttacking = false;
	bBeforeFreeze = false;
	bPullingAxe = false;
	bSprinting = false;
	health = 3;
	bombs = 0;
	maxBombs = 0;
	moons = 0;
	bStringOpen = false;
	bNextAttack = false;
	bSprintCameraRotation = true;
	bSprintCamera1st = false;
	bNextDodge = true;
	bequippedWeapon = false;
	bDodgeBufferWindow = false;
	bDodgeBuffer = false;
	bDodgeBufferExecute = false;
	bResurrecting = false;
	bMainMenu = true;
	bPickUpBomb = false;
	bOverlappingPickup = false;
	bReadyToDisplayPickUp = false;
	bDoubleSprint = true;
	bPause = false;
	reverseSubs = false;
	reverseEndScreen = false;
	binNPCRange = false;
	bInDialogue = false;
	bStopDialogue = false;
	bFallDeath = false;
	bReversePickedUp = false;
	bPickedUpMoon = false;
	bPickedUpBomb = false;
	bPickedUpAxe = false;
	bPickedUpBat = false;
	bBatHit = false;
	bStumble = false;


	


	//Interaction Volumes
	range = CreateDefaultSubobject<USphereComponent>(FName("range"));
	range->SetSphereRadius(800.f);
	range->SetupAttachment(GetCapsuleComponent());
	range->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	range->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	range->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	range->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	range->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

	interactionBox = CreateDefaultSubobject<UBoxComponent>(FName("Interaction Box"));
	interactionBox->SetBoxExtent(FVector(100.f, 60.f, 30.f));
	interactionBox->SetupAttachment(GetCapsuleComponent());
	interactionBox->SetRelativeLocation(FVector(100.f, 0.f, -60.f));
	interactionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	interactionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	interactionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	interactionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	interactionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

	kickCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("box"));
	kickCollision->SetupAttachment(GetMesh());
	kickCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("foot_l"));
	kickCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	kickCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	kickCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	kickCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	kickCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	kickCollision->SetRelativeLocation(FVector(15, 5.f, 0.f)); //15,5,0
	kickCollision->SetBoxExtent(FVector(25.f, 20.f, 20.f));
	kickCollision->bHiddenInGame = false;
/*
	//Body specific volumes
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
	*/
	// set our turn rate for input
	TurnRateGamepad = 60.f;

	TurnRateGamepadAiming = 50.f;
	
	movementStatus = EMovementStatus::EMS_Idle;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1000.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 1.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 700.f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->bDrawDebugLagMarkers = false;
	CameraBoom->CameraLagSpeed = 10.f;
	CameraBoom->CameraRotationLagSpeed = 20.f;
	CameraBoom->CameraLagMaxDistance = 100.f;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 150.f)); //0,0,70
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->FieldOfView = 50;

	freezeCapsuleRadius = 60;
	startArmLength = 700.f;
	toggleArmLength = 600.f;
	aimArmLength = 300.f;
	aimOffset = FVector(0.f, 50.f, -20.f);
	toggleOffset = FVector(0.f, 150.f, 0.f);
	cameraOffset = FVector(0.f, 100.f, 0.f);

	aimROffset = FRotator(0.f, -1.f, 0.f);
	toggleROffset = FRotator(0.f, -10.f, 0.f);
	cameraROffset = FRotator(0.f, 0.f, 0.f);

	spawnLocation = FVector(-9000, 4000, 500);

	mainPlayerController = Cast<AMainPlayerController>(GetController());


}

void ADefaultUnreal5Character::BeginPlay()
{

	Super::BeginPlay();

	mainPlayerController = Cast<AMainPlayerController>(GetController());

	range->OnComponentBeginOverlap.AddDynamic(this, &ADefaultUnreal5Character::RangeOnOverlapBegin);
	range->OnComponentEndOverlap.AddDynamic(this, &ADefaultUnreal5Character::RangeOnOverlapEnd);

	interactionBox->OnComponentBeginOverlap.AddDynamic(this, &ADefaultUnreal5Character::DialogueOverlapBegin);
	interactionBox->OnComponentEndOverlap.AddDynamic(this, &ADefaultUnreal5Character::DialogueOverlapEnd);


	kickCollision->OnComponentBeginOverlap.AddDynamic(this, &ADefaultUnreal5Character::KickOverlapBegin);

	animInstance = GetMesh()->GetAnimInstance();


	if (bMainMenu)
	{
		SetActorLocation(spawnLocation);
		SetActorRotation(FRotator(0.f));
		GetController()->SetControlRotation(FRotator(0.f));
	}

	//Timeline shit
	if (fCurve && fCurve2)
	{
		FOnTimelineFloat InterpFunction{};
		FOnTimelineEvent TimelineFinished{};
		InterpFunction.BindUFunction(this, FName("AimTimeLineFloatReturn"));
		TimelineFinished.BindUFunction(this, FName("OnAimTimelineFinished"));
		aimTimeLine->AddInterpFloat(fCurve2, InterpFunction, FName("Alpha"));
		aimTimeLine->SetTimelineFinishedFunc(TimelineFinished);
		aimTimeLine->SetLooping(false);
		aimTimeLine->SetIgnoreTimeDilation(true);
		
		FOnTimelineFloat interpToggleFunction{};
		FOnTimelineEvent toggleTimelineFinished{};
		interpToggleFunction.BindUFunction(this, FName("ToggledTimeLineFloatReturn"));
		toggleTimelineFinished.BindUFunction(this, FName("OnToggledTimelineFinished"));
		toggleTimeLine->AddInterpFloat(fCurve, interpToggleFunction, FName("Alpha"));
		toggleTimeLine->SetTimelineFinishedFunc(toggleTimelineFinished);
		toggleTimeLine->SetLooping(false);
		toggleTimeLine->SetIgnoreTimeDilation(true);

		FOnTimelineFloat InterpAimReverseFunction{};
		InterpAimReverseFunction.BindUFunction(this, FName("AimReverseTimeLineFloatReturn"));
		aimTimeLineReverse->AddInterpFloat(fCurve2, InterpAimReverseFunction, FName("Alpha"));
		aimTimeLineReverse->SetLooping(false);
		aimTimeLineReverse->SetIgnoreTimeDilation(true);

		FOnTimelineFloat InterToggleReverseFunction{};
		InterToggleReverseFunction.BindUFunction(this, FName("ToggledReverseTimeLineFloatReturn"));
		toggleTimeLineReverse->AddInterpFloat(fCurve, InterToggleReverseFunction, FName("Alpha"));
		toggleTimeLineReverse->SetLooping(false);
		toggleTimeLineReverse->SetIgnoreTimeDilation(true);

		FOnTimelineFloat InterSprintFunction{};
		FOnTimelineEvent SprintTimelineFinished{};
		SprintTimelineFinished.BindUFunction(this, FName("OnSprintTimelineFinished"));
		sprintTimeLine->SetTimelineFinishedFunc(SprintTimelineFinished);
		InterSprintFunction.BindUFunction(this, FName("SprintTimeLineFloatReturn"));
		sprintTimeLine->AddInterpFloat(fCurve2, InterSprintFunction, FName("Alpha"));
		sprintTimeLine->SetLooping(false);
		sprintTimeLine->SetIgnoreTimeDilation(true);
	}
}


void ADefaultUnreal5Character::RangeOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		inRangeEnemy.Add(OtherActor);
	}
}

void ADefaultUnreal5Character::RangeOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		inRangeEnemy.Remove(OtherActor);
	}
}

void ADefaultUnreal5Character::DialogueOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ANPCS>(OtherActor))
	{
		npc = Cast<ANPCS>(OtherActor);
		binNPCRange = true; 
		
		mainPlayerController->npcLocation = FVector(npc->GetMesh()->GetSocketLocation("neck_02").X, npc->GetMesh()->GetSocketLocation("neck_02").Y, npc->GetMesh()->GetSocketLocation("neck_02").Z + 80);
		toggledOnActor = OtherActor ;
	}
}

void ADefaultUnreal5Character::DialogueOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ANPCS>(OtherActor))
	{
		binNPCRange = false;
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ADefaultUnreal5Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADefaultUnreal5Character::Interact);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ADefaultUnreal5Character::Attack);

	PlayerInputComponent->BindAction("kick", IE_Pressed, this, &ADefaultUnreal5Character::Kick);

	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ADefaultUnreal5Character::DodgeAndSprint);
	PlayerInputComponent->BindAction("Dodge", IE_Released, this, &ADefaultUnreal5Character::StopSprint);

	PlayerInputComponent->BindAction("Freeze", IE_Pressed, this, &ADefaultUnreal5Character::Freeze);

	PlayerInputComponent->BindAction("TestingSpawnAxe", IE_Pressed, this, &ADefaultUnreal5Character::RealSpawnAxe);
	
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ADefaultUnreal5Character::AimOn);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ADefaultUnreal5Character::AimOff);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ADefaultUnreal5Character::ESCDown);

	PlayerInputComponent->BindAction("Toggle", IE_Pressed, this, &ADefaultUnreal5Character::ToggleOn);
	PlayerInputComponent->BindAction("Toggle Left", IE_Pressed, this, &ADefaultUnreal5Character::ToggleLeft);
	PlayerInputComponent->BindAction("Toggle Right", IE_Pressed, this, &ADefaultUnreal5Character::ToggleRight);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ADefaultUnreal5Character::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ADefaultUnreal5Character::MoveRight);
		
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ADefaultUnreal5Character::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ADefaultUnreal5Character::LookUpAtRate);
}


void ADefaultUnreal5Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Do toggled on camera
	if (bToggledOn && movementStatus != EMovementStatus::EMS_Dead && bNotDead)
	{
		//FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(toggledOnActor->GetActorLocation().X, toggledOnActor->GetActorLocation().Y, toggledOnActor->GetActorLocation().Z - 100.f));
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(toggledOnActor->GetActorLocation().X, toggledOnActor->GetActorLocation().Y, toggledOnActor->GetActorLocation().Z ));
		FRotator rotationInterp = UKismetMathLibrary::RInterpTo(GetActorRotation(), targetRotation, DeltaTime, 3.f);
		rotationInterp = rotationInterp.GetNormalized();
		FRotator playerRotation = GetActorRotation().GetNormalized();
		FRotator newControllerRot = FRotator(rotationInterp.Pitch, rotationInterp.Yaw, playerRotation.Roll);
		GetController()->SetControlRotation(newControllerRot);
		SetActorRotation(FRotator(playerRotation.Pitch, newControllerRot.Yaw, playerRotation.Roll));

		if (bToggledOn && mainPlayerController)
		{
			if(enemy->bear)
			mainPlayerController->enemyLocation = enemy->GetMesh()->GetSocketLocation("b_spine_03");
			else
			mainPlayerController->enemyLocation = enemy->GetMesh()->GetSocketLocation("spine_04");
		}
	}

	// Fix Up for happenstance camera Bug in rotation
	if (GetControlRotation().Roll != 0)
	{
		GetController()->SetControlRotation(FRotator(GetControlRotation().Pitch, GetControlRotation().Yaw, 0));
	}

	//	Do Neck rotation and untoggle from enemy when it dies
	if (bToggledOn || binNPCRange)
	{
		NeckRotator();

		if (enemy)
		{
			if (enemy->health < 1)
			{
				ToggleOff();
			}
		}
	
	}


//UI PickUp Display
	if (bOverlappingPickup && bNotDead)
	{
	
		TArray<AActor*> overlap;
		GetCapsuleComponent()->GetOverlappingActors(overlap);
		for (int i = 0; i < overlap.Num(); i++)
		{
			if (overlap[i])
			{

				if (Cast<AAxe>((overlap[i])) && Cast<AAxe>((overlap[i])) != equippedAxe)
				{
				//	UE_LOG(LogTemp, Warning, TEXT("Overlapping"));
					bOverlappingPickup = true;
					
					if (mainPlayerController)
					{
						mainPlayerController->DisplayPickUp();
					}

				}
				else if (Cast<ABat>((overlap[i])) && Cast<ABat>((overlap[i])) != equippedBat)
				{
					activeOverlappedBat = Cast<ABat>((overlap[i]));
					bOverlappingPickup = true;
					if (mainPlayerController)
					{
						mainPlayerController->DisplayPickUp();
					
					}

				}
				else if (Cast<AStaticPickUp>(overlap[i]))
				{
					activeOverlappedStatic = Cast<AStaticPickUp>((overlap[i]));
					bOverlappingPickup = true;
					if (mainPlayerController)
					{
						mainPlayerController->DisplayPickUp();
			
					}
				}	
				else
				{
					bOverlappingPickup = false;
					mainPlayerController->RemovePickUp();
				}
			}
			else
			{	
		//		activeOverlappedStatic = nullptr;
		//		activeOverlappedBat = nullptr;
				bOverlappingPickup = false;
				mainPlayerController->RemovePickUp();
			}
			
		}

	}
	else
	{

//		activeOverlappedStatic = nullptr;
//		activeOverlappedBat = nullptr;
		mainPlayerController->RemovePickUp();
	}

	if (bInDialogue)
	{
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(npc->GetActorLocation().X, npc->GetActorLocation().Y, npc->GetActorLocation().Z));
		FRotator rotationInterp = UKismetMathLibrary::RInterpTo(GetActorRotation(), targetRotation, DeltaTime, 100.f);
		rotationInterp = rotationInterp.GetNormalized();
		FRotator playerRotation = GetActorRotation().GetNormalized();
		FRotator newControllerRot = FRotator(rotationInterp.Pitch, rotationInterp.Yaw, playerRotation.Roll);
		GetController()->SetControlRotation(newControllerRot);
	}
	

	//dodge on buffer to do it ahead of time
	if (bDodgeBufferExecute)
	{
		bNextDodge = true;
		bDodgeBufferWindow = false;
		bDodgeBuffer = false;
		DodgeAndSprint();
		bDodgeBufferExecute = false;
	//	UE_LOG(LogTemp, Warning, TEXT("%f"), GetInputAxisValue(FName("Turn Right / Left Mouse")) + GetInputAxisValue(FName("Look Up / Down Mouse")) + GetInputAxisValue(FName("Turn Right / Left Gamepad")) + GetInputAxisValue(FName("Look Up / Down Gamepad")));
	}
	
	//Camera correction on player run
	if(!bInDialogue && movementStatus != EMovementStatus::EMS_Idle && (GetInputAxisValue(FName("Turn Right / Left Mouse")) + GetInputAxisValue(FName("Look Up / Down Mouse")) + GetInputAxisValue(FName("Turn Right / Left Gamepad")) + GetInputAxisValue(FName("Look Up / Down Gamepad")) == 0) && (deltaDirection.Yaw < 160 && deltaDirection.Yaw > -160))
	{
		SprintCamera(bSprintCamera1st);
	}

}

void ADefaultUnreal5Character::Resurrect()
{
	FTimerHandle UnusedHandle;
	FTimerHandle UnusedHandle2;
	FTimerHandle UnusedHandle3;

	mainPlayerController->DisplayDeathScreen();
	bResurrecting = true;

	movementStatus = EMovementStatus::EMS_Idle; 
	health = 3;
		
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ADefaultUnreal5Character::DestroyAndSpawnEnemies, 1.f, false);

}


void ADefaultUnreal5Character::SprintCamera(bool first)
{
	if (!bToggledOn && bNotDead)
	{
		startControlRotation = GetControlRotation().Yaw;
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000);
		FRotator camera = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FollowCamera->GetRelativeLocation());
		FRotator playerRotation = GetActorRotation().GetNormalized();
		sprintControllerRot = FRotator(GetControlRotation().Pitch, targetRotation.Yaw, GetControlRotation().Roll);

		if (movementStatus == EMovementStatus::EMS_Walk)
		{
			GetController()->SetControlRotation(FRotator(FQuat::Slerp(GetControlRotation().Quaternion(), sprintControllerRot.Quaternion(), .004)));
		}
		else if (movementStatus == EMovementStatus::EMS_Run)
		{
			GetController()->SetControlRotation(FRotator(FQuat::Slerp(GetControlRotation().Quaternion(), sprintControllerRot.Quaternion(), .01)));
		}
		else if (movementStatus == EMovementStatus::EMS_Sprint)
		{
			GetController()->SetControlRotation(FRotator(FQuat::Slerp(GetControlRotation().Quaternion(), sprintControllerRot.Quaternion(), .05)));
		}
	}
}


void ADefaultUnreal5Character::RealSpawnAxe()
{
	FActorSpawnParameters spawnInfo;
	AActor* newAxe = GetWorld()->SpawnActor<AActor>(axeBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
	equippedAxe = Cast<AAxe>(newAxe);
	equippedAxe->Equip(this, true);
	equippedAxe->pullCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bequippedWeapon = true;
}

void ADefaultUnreal5Character::StartGame()
{
	mainPlayerController->RemoveMainMenu();
	mainPlayerController->GameModeOnly();
}

void ADefaultUnreal5Character::AimTimeLineFloatReturn(float value)
{
	CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, aimArmLength, value);

	CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset, aimOffset, value);

	FollowCamera->SetRelativeRotation(FMath::Lerp(FollowCamera->GetRelativeRotation(), aimROffset, value));
}

void ADefaultUnreal5Character::AimReverseTimeLineFloatReturn(float value)
{
	CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength , startArmLength, value);

	CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset, cameraOffset, value);

	FollowCamera->SetRelativeRotation(FMath::Lerp(FollowCamera->GetRelativeRotation(), cameraROffset, value));
}

void ADefaultUnreal5Character::OnSprintTimelineFinished()
{
	bSprintCamera1st = false;
}

void ADefaultUnreal5Character::ToggledTimeLineFloatReturn(float value)
{
	CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, toggleArmLength, value);

	CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset, toggleOffset, value);

	FollowCamera->SetRelativeRotation(FMath::Lerp(FollowCamera->GetRelativeRotation(), toggleROffset, value));
}

void ADefaultUnreal5Character::ToggledReverseTimeLineFloatReturn(float value)
{
	CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, startArmLength, value);

	CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset, cameraOffset, value);

	FollowCamera->SetRelativeRotation(FMath::Lerp(FollowCamera->GetRelativeRotation(), cameraROffset, value));
}


float ADefaultUnreal5Character::GetWalkingDirectionAngle() 
{
	if (!bFrozen && FollowCamera && movementStatus != EMovementStatus::EMS_Dead && bNotDead)
	{
		float emptyX;
		float emptyY;
		deltaDirection = UKismetMathLibrary::NormalizedDeltaRotator(FollowCamera->GetComponentRotation() , GetCapsuleComponent()->GetComponentRotation());
	
		FRotator deltaMovement = UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::MakeVector(GetInputAxisValue(FName("Move Forward / Backward")), GetInputAxisValue(FName("Move Right / Left"))*-1, 0.f));
		UKismetMathLibrary::BreakRotator(UKismetMathLibrary::NormalizedDeltaRotator(deltaDirection, deltaMovement), emptyX, emptyY, direction);

		return direction;
	}
	return 0.f;
}	

void ADefaultUnreal5Character::StopSprint()
{
	if (!bDodgeBuffer) {
		bSprinting = false;
	}
	else 
	{
		bDoubleSprint = false;
	}
}

void ADefaultUnreal5Character::DodgeAndSprint()
{
	FTimerHandle UnusedHandle;
	//GetWorldTimerManager().SetTimer(UnusedHandle, this, &ADefaultUnreal5Character::SprintCamera, 1.f, false);
	if (bDodgeBufferWindow)
	{
		bDodgeBuffer = true;
	//	UE_LOG(LogTemp, Warning, TEXT("sex2"));
	}
	if (!bFreezing && !bStumble && !bFrozen && bNextDodge && !bAttacking && movementStatus != EMovementStatus::EMS_Dead && bNotDead)
	{

	//	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		
		if (animInstance)
		{
			bDodge = true;
			bNextDodge = false;

//			if(bDoubleSprint)
				bSprinting = true;

			bDoubleSprint = true;
			bSprintCameraRotation = true;
//			movementStatus = EMovementStatus::EMS_Sprint;
		//	UE_LOG(LogTemp, Warning, TEXT("penis"));
			if (!bToggledOn)
			{
			//	UE_LOG(LogTemp, Warning, TEXT("penis"));
				animInstance->Montage_Play(characterAnims, 1.f);
				//GetActorRotation().
				SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw+direction, GetActorRotation().Roll));
				animInstance->Montage_JumpToSection(FName("Dodge"));
			}

			if(bAiming)
				AimOff();
		}
	}
}

void ADefaultUnreal5Character::ToggleOn() 
{
	if (movementStatus != EMovementStatus::EMS_Dead && bToggledOn)
	{
		ToggleOff();
	
		return;
	}

	if (!bStumble && movementStatus != EMovementStatus::EMS_Dead && !bAiming && !bToggledOn && bNotDead)
	{
		
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
			ObjectTypesArray.Reserve(1);
			ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle));
			TArray<AActor*> ignoredActors;

			FHitResult hit;
		
			UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),GetActorLocation(), FollowCamera->GetForwardVector()*2000 + GetActorLocation(), 300.f, ObjectTypesArray, false, ignoredActors, EDrawDebugTrace::None, hit, true);

			if (hit.GetActor())
			{
				toggledOnActor = hit.GetActor();
				enemy = Cast<AEnemy>(toggledOnActor);
				bToggledOn = true;

				bUseControllerRotationYaw = true;
				GetCharacterMovement()->bOrientRotationToMovement = false;
			//	CameraBoom->CameraRotationLagSpeed = 15;
				CameraBoom->CameraLagSpeed = 4;
				
			//	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
				UPlayerAnimInstance* playerAnim = Cast< UPlayerAnimInstance >(animInstance);
				playerAnim->bLocked = true;
			

				if (mainPlayerController)
				{
					mainPlayerController->DisplayToggle();
				}

				toggleTimeLine->PlayFromStart();
				return;
			}	
	}
}

void ADefaultUnreal5Character::ToggleOff()
{
	bToggledOn = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	toggledOnActor = nullptr;
//	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	UPlayerAnimInstance* playerAnim = Cast< UPlayerAnimInstance >(animInstance);
	playerAnim->bLocked = false;\

	if (mainPlayerController)
	{
		mainPlayerController->RemoveToggle();
		//mainPlayerController->RemovePickUp();
	}
	if(!bAiming)
	{
		toggleTimeLineReverse->PlayFromStart();
	}
	
}

void ADefaultUnreal5Character::ToggleLeft() 
{
	
	if (!bStumble && movementStatus != EMovementStatus::EMS_Dead  && bToggledOn)
	{
		TArray<AActor*> AIonRight;
		closestAIToTarget = nullptr;
		AIonRight.Empty();

		for (auto Actor : inRangeEnemy)
		{
			if (Actor != toggledOnActor)
			{
				
				FVector rightOfEnemy = toggledOnActor->GetActorLocation() - Actor->GetActorLocation();
				FVector rightOfPlayer = GetActorRightVector();
				float dotProduct = UKismetMathLibrary::Dot_VectorVector(rightOfPlayer, rightOfEnemy);

				if (dotProduct > 0)
				{
					AIonRight.Add(Actor);
				}
			}
		}

		if(AIonRight.Num() > 0)
		{
			if (AIonRight.GetData())
			{
				closestAIToTarget = AIonRight[0];


				for (auto Actor : AIonRight)
				{
					if (Actor != toggledOnActor)
					{
						float enemiesDistance = Actor->GetDistanceTo(toggledOnActor);
						float enemiesDistance2 = closestAIToTarget->GetDistanceTo(toggledOnActor);

						if (enemiesDistance < enemiesDistance2)
						{
							closestAIToTarget = Actor;
						}
					}
				}

			}
			toggledOnActor = closestAIToTarget;
			enemy = Cast<AEnemy>(toggledOnActor);
		}
	}
}

void ADefaultUnreal5Character::ToggleRight() 
{
	if (!bStumble && movementStatus != EMovementStatus::EMS_Dead  && bToggledOn)
	{
		TArray<AActor*> AIonLeft;
		AIonLeft.Empty();
		closestAIToTarget = nullptr;

			for (auto Actor : inRangeEnemy)
			{
				if (Actor != toggledOnActor)
				{

					FVector leftOfEnemy = toggledOnActor->GetActorLocation() - Actor->GetActorLocation();
					FVector leftOfPlayer = GetActorRightVector();
					float dotProduct = UKismetMathLibrary::Dot_VectorVector(leftOfPlayer, leftOfEnemy.GetSafeNormal());

					if (dotProduct <= 0)
					{
						AIonLeft.Add(Actor);
					}
				}
			}

			if (AIonLeft.Num() > 0)
			{
				if (AIonLeft.GetData())
				{
					closestAIToTarget = AIonLeft[0];

					for (auto Actor : AIonLeft)
					{
						if (Actor != toggledOnActor)
						{
							float enemiesDistance = Actor->GetDistanceTo(toggledOnActor);
							float enemiesDistance2 = closestAIToTarget->GetDistanceTo(toggledOnActor);

							if (enemiesDistance < enemiesDistance2)
							{
								closestAIToTarget = Actor;
							}
						}
					}
				}

				toggledOnActor = closestAIToTarget;
				enemy = Cast<AEnemy>(toggledOnActor);
			}
	}
}

void ADefaultUnreal5Character::NeckRotator()
{
	FRotator lookAtRot;
	ACharacter* toggled;

	if (toggledOnActor)
	{
		toggled = Cast<ACharacter>(toggledOnActor);

		lookAtRot = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(FName("neck_02")), toggled->GetMesh()->GetSocketLocation(FName("neck_02")));

		if (neckRotYaw > 180 || neckRotYaw < -180)
		{
			neckRotYaw = lookAtRot.Yaw - GetActorRotation().Yaw;
		}
		else
		{
			neckRotYaw = GetActorRotation().Yaw - lookAtRot.Yaw;
		}

	//	UE_LOG(LogTemp, Warning, TEXT("Yaw = %f"), neckRotYaw);
		neckRotPitch = GetActorRotation().Pitch - lookAtRot.Pitch;
	//	UE_LOG(LogTemp, Warning, TEXT("Pitch = %f"), neckRotPitch);

	}
}



void ADefaultUnreal5Character::DestroyAndSpawnEnemies()
{


	bMovingForward = false;
	bDodge = false;
	bToggledOn = false;
	bMovingRight = false;
	bAiming = false;
	bAttacking = false;
	bBeforeFreeze = false;
	bPullingAxe = false;
	bSprinting = false;

	bombs = 0;
	bombsStaticMesh->SetVisibility(false);

	bStringOpen = false;
	bNextAttack = false;
	bSprintCameraRotation = true;
	bSprintCamera1st = false;
	bNextDodge = true;
	bequippedWeapon = false;
	bDodgeBufferWindow = false;
	bDodgeBuffer = false;
	bDodgeBufferExecute = false;
	bDoubleSprint = false;
	binNPCRange = false;
	bInDialogue = false;
	bStopDialogue = false;
	bFreezing = false;
	bStumble = false;
	bFrozen = false;

	GetMesh()->SetCollisionProfileName(FName("Pawn"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	animInstance->Montage_Stop(0.2f, characterAnims);
	bNotDead = true;
	bResurrecting = false;


	AEnemySpawnPoint* enemySpawned;
	TArray<AActor*> enemySpawnPoints;

	AStaticPickUpSpawnPoint* itemSpawnPoint;
	TArray<AActor*> ItemSpawnPoints;

	//AEnemySpawnPoint* spawnPoint;
	TArray<AActor*> axePoints;

	//AStaticPickUpSpawnPoint* itemSpawnPoint;
	TArray<AActor*> batPoints;

	SetActorLocation(FVector(spawnLocation));
	SetActorRotation(FRotator(0));
	GetController()->SetControlRotation(FRotator(0));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), enemySpawnBlueprint, enemySpawnPoints);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), pickUpSpawnBlueprint, ItemSpawnPoints);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), axeBlueprint, axePoints);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), batBlueprint, batPoints);
	
	if (bFallDeath)
	{
		movementStatus = EMovementStatus::EMS_Idle;
		bFallDeath = false;
	}
	else
	{
		reverseSubs = true;
		mainPlayerController->RemoveSubs();
	}
		
	
	if (!axePoints.IsEmpty())
	{
		for (auto Actor : axePoints)
		{
			if(!Cast<AAxe>(Actor)->bDoor)
				Cast<AAxe>(Actor)->Destroy();
			//spawnPoint->DestroySpawned();
		}
	}

	if (!batPoints.IsEmpty())
	{
		for (auto Actor : batPoints)
		{
			Cast<ABat>(Actor)->Destroy();
		}
		
	}

	if (!enemySpawnPoints.IsEmpty())
	{
		for (auto Actor : enemySpawnPoints)
		{
			enemySpawned = Cast<AEnemySpawnPoint>(Actor);

			if(	enemySpawned)
			{
				enemySpawned->DestroySpawned();
				enemySpawned->Spawn();
			}
			
		}
	}
	


	if (!ItemSpawnPoints.IsEmpty())
	{
		for (auto Actor : ItemSpawnPoints)
		{
			itemSpawnPoint = Cast<AStaticPickUpSpawnPoint>(Actor);
			if (itemSpawnPoint)
			{
				itemSpawnPoint->DestroySpawned();
//				itemSpawnPoint->Spawn();
			}
			
		}
	}

}

void ADefaultUnreal5Character::AimOn() 
{
	if (!bStumble && !bFrozen && (bequippedWeapon || bombs > 0) && movementStatus != EMovementStatus::EMS_Dead && bNotDead)
	{
		bAiming = true;

		if (bToggledOn)
		{
			ToggleOff();
		}
		if (mainPlayerController)
		{
			mainPlayerController->DisplayCrossHair();
			//mainPlayerController->TogglePauseMenu();
		}
//		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		UPlayerAnimInstance* playerAnim = Cast< UPlayerAnimInstance >(animInstance);
		if (!bequippedWeapon)
		{
			bombsStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("RightHandSo"));
			bombsStaticMesh->SetRelativeLocation(FVector(0.f));
		//	bombsStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("pelvis"));
		}
		playerAnim->bLocked = true;
		TurnRateGamepad = TurnRateGamepadAiming;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 1.f, 0.0f);
		CameraBoom->bEnableCameraLag = false;
		CameraBoom->bEnableCameraRotationLag = false;
		bUseControllerRotationYaw = true;

		aimTimeLine->PlayFromStart();
		
		UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMin = -40;
		UGameplayStatics::GetPlayerCameraManager(this,0)->ViewPitchMax = 40;
	}
}


void ADefaultUnreal5Character::AimOff()
{
	if (bAiming)
	{
//		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		UPlayerAnimInstance* playerAnim = Cast< UPlayerAnimInstance >(animInstance);
		bAiming = false;
		playerAnim->bLocked = false;
		bUseControllerRotationYaw = false;
		CameraBoom->bEnableCameraLag = true;
		CameraBoom->bEnableCameraRotationLag = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
		TurnRateGamepad = 60.f;
		if (mainPlayerController)
		{
			mainPlayerController->RemoveCrossHair();
			//mainPlayerController->TogglePauseMenu();
		}

		if (!bequippedWeapon)
		{
		//	bombsStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("RightHandSo"));
			bombsStaticMesh->SetRelativeLocation(FVector(10.f, -17.f, 10.f));
			bombsStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform), FName("pelvis"));
		}

		aimTimeLineReverse->PlayFromStart();
		
		UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMin = -90;
		UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMax = 90;
	}
}

void ADefaultUnreal5Character::Interact()
{
	//	FActorSpawnParameters spawnInfo;
		//GetWorld()->SpawnActor<AAxe>(GetActorLocation(), FRotator(0.f), spawnInfo);&& binAxeRange

	if (bInDialogue)
	{
		StopDialogue();
	}
	else if (binNPCRange && !bInDialogue)
	{
		StartDialogue();
	}

	if (!bStumble && !bFrozen && (activeOverlappedAxe || activeOverlappedBat || activeOverlappedStatic) && movementStatus != EMovementStatus::EMS_Dead && bNotDead)
	{
		//	UE_LOG(LogTemp, Warning, TEXT("InteractPressed"));
		//	Pulling from head or door
		if (headAxe && !bPullingAxe)
		{
			UPlayerAnimInstance* playerAnim = Cast< UPlayerAnimInstance >(animInstance);
			animInstance->Montage_Play(characterAnims, 1.0f);
			animInstance->Montage_JumpToSection(FName("pull"), characterAnims);
		//	UE_LOG(LogTemp, Warning, TEXT("PulFromHead"));
			bPullingAxe = true;
			playerAnim->pullingAxe = activeOverlappedAxe;
			activeOverlappedAxe->PullAxeAnimation();
		}
		//picking up from the ground
		else if (!bPullingAxe && !bequippedWeapon && (activeOverlappedAxe || activeOverlappedBat))
		{
			UE_LOG(LogTemp, Warning, TEXT("PickedUp"));
			PickUp();
				if (activeOverlappedAxe)
				{
					AxePickUp();
				}
				else if (activeOverlappedBat)
					BatPickUp();

			
		}
		else if ( activeOverlappedStatic)
		{
			if (bPickUpBomb && bombs < 1)
			{
			//	UE_LOG(LogTemp, Warning, TEXT("PickedUp2"));
				bombs++;
				bPickUpBomb = false;
				bPickedUpBomb = true;
				//	bombsStaticMesh[0]->SetVisibility(true, true);
				//	bombsStaticMesh[bombsStaticMesh.Num()-1]->SetRelativeLocation(FVector(10.f,-16.f,10.f));
				bombsStaticMesh->SetVisibility(true);
				PickUp();
				
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), pickUpSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
				activeOverlappedStatic->Destroy();
			}	
			else 
			{
				moons++;
				bPickedUpMoon = true;
				PickUp();
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), pickUpSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
				activeOverlappedStatic->Destroy();
			}
		}
		
	}
}


void ADefaultUnreal5Character::AxePickUp()
{
	//if(!bPullingAxe)
		bequippedWeapon = true;

	if (activeOverlappedAxe)
	{

		UE_LOG(LogTemp, Warning, TEXT("PickedUp"));
		SetEquippedAxe(activeOverlappedAxe);
		activeOverlappedAxe->Equip(this, true);
		bPickedUpAxe = true;
		//	headAxe = false;
		//	equippedAxe->Equip(this);
	}
	//UE_LOG(LogTemp, Warning, TEXT("nice4"));
	SetOverlappingAxe(nullptr, false);
}

void ADefaultUnreal5Character::BatPickUp()
{
	bequippedWeapon = true;
	if (activeOverlappedBat)
	{
		SetEquippedBat(activeOverlappedBat);
		activeOverlappedBat->Equip(this, true);
		bPickedUpBat = true;
	//	headAxe = false;
	//	equippedAxe->Equip(this);
	}
	
	SetOverlappingBat(nullptr);
}

void ADefaultUnreal5Character::Freeze()
{
//	UE_LOG(LogTemp, Warning, TEXT("kick hit"));
	if (!bStumble && movementStatus != EMovementStatus::EMS_Dead && !bFreezing && bNotDead && !bDodge && !bAttacking)
	{
//		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (bAiming)
		{
			AimOff();
		}
		if (animInstance)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, screamVFX, GetActorLocation(), FRotator(0.f));

			GetCapsuleComponent()->SetCapsuleRadius(freezeCapsuleRadius);
			bFreezing = true;
			animInstance->Montage_Play(characterAnims, 1.0f);
			animInstance->Montage_JumpToSection(FName("Freeze"), characterAnims);

			UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), parryingSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
		}
	}
}

void ADefaultUnreal5Character::Attack() 
{

	if ( movementStatus != EMovementStatus::EMS_Dead && bNotDead && !bDodge)
	{
		
	if (bStringOpen)
	{
		bNextAttack = true;
	}
//		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

		if (bequippedWeapon && !bStumble && !bFrozen && animInstance && !bAttacking)
		{
		//	UE_LOG(LogTemp, Warning, TEXT("nice4"));
			if (bAiming)
			{
				animInstance->Montage_Play(characterAnims, 1.5f);
				animInstance->Montage_JumpToSection(FName("Throw"), characterAnims);
				bAttacking = true;
			}
			else
			{
					animInstance->Montage_Play(characterAnims, 1.5f);
					animInstance->Montage_JumpToSection(FName("basicAttack"), characterAnims);
					bAttacking = true;
					bNextAttack = false;
				
				if (equippedAxe->swingSound)
				{
					//	UGameplayStatics::PlaySoundAtLocation();
				}
			}
		}
			else if (!bequippedWeapon && !bStumble && !bFrozen && animInstance && !bAttacking)
			{
		//	UE_LOG(LogTemp, Warning, TEXT("nice4"));
			if (headAxe  && !bPullingAxe)
			{
				UPlayerAnimInstance* playerAnim = Cast< UPlayerAnimInstance >(animInstance);
				animInstance->Montage_Play(characterAnims, 1.0f);
				animInstance->Montage_JumpToSection(FName("pull"), characterAnims);
				UE_LOG(LogTemp, Warning, TEXT("pullingaxe"));
				bPullingAxe = true;
				playerAnim->pullingAxe = activeOverlappedAxe;
				activeOverlappedAxe->PullAxeAnimation();
			}
			else if (bombs > 0 && bAiming)
			{
				
				BombSpawn();
				

				animInstance->Montage_Play(characterAnims, 1.5f);
				animInstance->Montage_JumpToSection(FName("Throw"), characterAnims);
			//	bombs--;
				bAttacking = true;
			}
		}
	}
}

void ADefaultUnreal5Character::PickUp()
{
	if (movementStatus != EMovementStatus::EMS_Dead && bNotDead)
	{
		//		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (!bStumble && !bFrozen && animInstance && !bAttacking)
		{
			mainPlayerController->DisplayPickedUpItem();
			animInstance->Montage_Play(characterAnimsUpper, 1.f);
			animInstance->Montage_JumpToSection(FName("PickUp"), characterAnimsUpper);
		//	bAttacking = true;
		}
	}
}

void ADefaultUnreal5Character::Kick()
{
	if (movementStatus != EMovementStatus::EMS_Dead && bNotDead)
	{
		//		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (!bStumble && !bFreezing && animInstance && !bAttacking)
		{
			animInstance->Montage_Play(characterAnims, 1.f);
			animInstance->Montage_JumpToSection(FName("kick"), characterAnims);
			bAttacking = true;
		}
	}
}

void ADefaultUnreal5Character::KickOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("kick hit"));
		combatInstigator = this->GetInstigatorController();
		UGameplayStatics::ApplyDamage(OtherActor, 8, combatInstigator, this, damageTypeClass);
		kickCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void ADefaultUnreal5Character::SetupKickCollision(bool activate)
{

	if (activate)
	{
		kickCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//		UE_LOG(LogTemp, Warning, TEXT("activate kick"));
	}
	else
	{
		kickCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//			UE_LOG(LogTemp, Warning, TEXT("deactivate kick"));
	}

}

void ADefaultUnreal5Character::BombSpawn()
{
		bombsStaticMesh->SetVisibility(false);
		FActorSpawnParameters spawnInfo;
		AActor* newBomb = GetWorld()->SpawnActor<AActor>(bombBlueprint, GetActorLocation(), GetActorRotation(), spawnInfo);
		equippedBomb = Cast<AIceBomb>(newBomb);
		equippedBomb->EquipBomb(this);
}


void ADefaultUnreal5Character::Throw()
{
	FVector startLocation = FollowCamera->GetForwardVector() * 200 + FollowCamera->GetUpVector() * 40 + GetMesh()->GetSocketLocation(FName("RightHandSo") );
	FVector endLocation = FollowCamera->GetForwardVector() * 5000 + FollowCamera->GetComponentLocation();
	
	FHitResult hit;
	FCollisionQueryParams params;
	FCollisionResponseParams rParams;
	params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(hit, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params, rParams);
//	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, true, 0.f, 0, 2.0f);
	if (bequippedWeapon)
	{
		if (equippedAxe)
		{
			if (hit.bBlockingHit)
			{
				
				equippedAxe->Thrown(hit.ImpactPoint - startLocation, GetActorRotation(), false, startLocation);
				bequippedWeapon = false;
			}
			else
			{
				equippedAxe->Thrown(endLocation - startLocation, GetActorRotation(), false, startLocation);
				bequippedWeapon = false;
			}

			equippedAxe = nullptr;
		}
		else if (equippedBat)
		{
			if (hit.bBlockingHit)
			{
				equippedBat->Thrown(hit.ImpactPoint - startLocation, GetActorRotation(), false, startLocation);
				bequippedWeapon = false;
			}
			else
			{
				equippedBat->Thrown(endLocation - startLocation, GetActorRotation(), false, startLocation);
				bequippedWeapon = false;
			}
			equippedBat = nullptr;
		}
		
	}
	else if(bombs > 0)
	{
 	   endLocation = FollowCamera->GetForwardVector() * 5000 + FollowCamera->GetComponentLocation();

		if (hit.bBlockingHit)
		{
			equippedBomb->Thrown(hit.ImpactPoint - startLocation, GetActorRotation(), false);
			bequippedWeapon = false;

		}
		else
		{
			equippedBomb->Thrown(endLocation - startLocation, GetActorRotation(), false);
			bequippedWeapon = false;
		}
		bombs -= 1;
		if (bombs <= 0)
		{
		
			AimOff();
		}

	}

	if (bequippedWeapon && bombs <= 0)
	{
		AimOff();
	}
}


void ADefaultUnreal5Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
//	UE_LOG(LogTemp, Warning, TEXT("ValueForward: %f"), Rate);
	yawTurnRate = Rate;

	//Turn Right / Left Mouse

}

void ADefaultUnreal5Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
	pitchTurnRate = Rate;

}

void ADefaultUnreal5Character::MoveForward(float Value)
{
	bMovingForward = false;
	//&& (Value != 0.0f)
//	UE_LOG(LogTemp, Warning, TEXT("ValueForward: %f"), Value);
	if (!bStumble && !bBeforeFreeze && !bAttacking && !bDodge && !bFrozen  && (Controller != nullptr)  && !bFreezing && bNotDead && !bPullingAxe && (Value != 0.0f) )
	{
	
		FName currentSection = animInstance->Montage_GetCurrentSection(characterAnims);

		animInstance->Montage_Stop(0.2f,characterAnims);
			
		bMovingForward = true;
	//	bNextDodge = true;
		
		if (UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Forward / Backward"))) + UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Right / Left"))) == 0)
		{
			movementStatus = EMovementStatus::EMS_Idle;
		}
		else if (bSprinting)
		{
			movementStatus = EMovementStatus::EMS_Sprint;
			//bRunning = true;
		}
		else if (UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Forward / Backward"))) + UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Right / Left"))) > 0.5f)// || (GetInputAxisValue(FName("Move Forward / Backward")) + GetInputAxisValue(FName("Move Right / Left"))) < -0.5f)
		{
			movementStatus = EMovementStatus::EMS_Run;

		}

		else if (UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Forward / Backward"))) + UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Right / Left"))) <= 0.5f) //|| (GetInputAxisValue(FName("Move Forward / Backward")) + GetInputAxisValue(FName("Move Right / Left"))) >= -0.5f)
		{
			movementStatus = EMovementStatus::EMS_Walk;
		}
			
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		//	UE_LOG(LogTemp, Warning, TEXT("ValueForward: %f"), YawRotation);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		
	}
	else if (!bMovingRight) {
		movementStatus = EMovementStatus::EMS_Idle;
		//bRunning = false;
	}
}

void ADefaultUnreal5Character::MoveRight(float Value)
{
	
	bMovingRight = false;
//	UE_LOG(LogTemp, Warning, TEXT("ValueRight: %f"), Value);
	//
	if (!bFrozen && !bStumble && !bBeforeFreeze && !bDodge && !bAttacking && (Controller != nullptr)  && !bFreezing && bNotDead && !bPullingAxe && (Value != 0.0f))
	{
		
		bMovingRight = true;
	//	bNextDodge = true;
		animInstance->Montage_Stop(0.2f, characterAnims);

		if (UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Forward / Backward"))) + UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Right / Left"))) == 0)
		{
			movementStatus = EMovementStatus::EMS_Idle;
		}
		else if (bSprinting)
		{
			movementStatus = EMovementStatus::EMS_Sprint;
			//bRunning = true;
		}
		else if (UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Forward / Backward"))) + UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Right / Left"))) > 0.5f)// || (GetInputAxisValue(FName("Move Forward / Backward")) + GetInputAxisValue(FName("Move Right / Left"))) < -0.5f)
		{
			movementStatus = EMovementStatus::EMS_Run;

		}

		else if (UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Forward / Backward"))) + UKismetMathLibrary::Abs(GetInputAxisValue(FName("Move Right / Left"))) <= 0.5f) //|| (GetInputAxisValue(FName("Move Forward / Backward")) + GetInputAxisValue(FName("Move Right / Left"))) >= -0.5f)
		{
			movementStatus = EMovementStatus::EMS_Walk;
		}
	
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
	else if (!bMovingForward) {
		movementStatus = EMovementStatus::EMS_Idle;
		//bRunning = false;
	}
}


void ADefaultUnreal5Character::Stumbling()
{

	if (bNotDead) {
		animInstance->Montage_Play(characterAnims, 1.f);
		animInstance->Montage_JumpToSection(FName("Stumble"), characterAnims);
		bStumble = true;
		bAttacking = false;
		bDodge = false;
		bBeforeFreeze = false;
		bFreezing = false;
		//	UE_LOG(LogTemp, Warning, TEXT("Stumbling"));
	}
}

void ADefaultUnreal5Character::BatHit()
{

	if (bNotDead) {
		animInstance->Montage_Play(characterAnims, 1.f);
		animInstance->Montage_JumpToSection(FName("batHit"), characterAnims);
		bBatHit = true;
		bStumble = true;
		bAttacking = false;
		bDodge = false;
		bBeforeFreeze = false;
		bFreezing = false;
		//	UE_LOG(LogTemp, Warning, TEXT("Stumbling"));
	}
}

void ADefaultUnreal5Character::Death()
{

	movementStatus = EMovementStatus::EMS_Dead;

	if (bFallDeath)
	{
	//	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		animInstance->Montage_Play(characterAnims, 1.f);
		animInstance->Montage_JumpToSection(FName("Death"), characterAnims);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

		Resurrect();
	}
	else
	{
	//	UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);
	
		animInstance->Montage_Play(characterAnims, 1.f);
		animInstance->Montage_JumpToSection(FName("Death"), characterAnims);

		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

		if (AlphaEnding)
		mainPlayerController->DisplayEndScreen();
		else
		mainPlayerController->DisplaySubs();
	}


	if (equippedAxe)
		equippedAxe->Drop();
	else if (equippedBat)
		equippedBat->Drop();
//	mainPlayerController->UIModeOnly();
	bequippedWeapon = false;
	//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	bNotDead = false;
	ToggleOff();
	
}

void ADefaultUnreal5Character::StopDialogue()
{
	npc->DialogueTrigger(true, this);
	bInDialogue = false;
	bStopDialogue = true;
//	mainPlayerController->RemoveSubs();

	CameraBoom->TargetArmLength = startArmLength;
	CameraBoom->SocketOffset = cameraOffset;
	FollowCamera->SetRelativeRotation(cameraROffset);
}

void ADefaultUnreal5Character::StartDialogue()
{
	npc->DialogueTrigger(false, this);
	bInDialogue = true;

	mainPlayerController->DisplaySubs();
	//	toggleTimeLine->PlayFromStart();
	CameraBoom->TargetArmLength = 300;
	CameraBoom->SocketOffset = FVector(0.f, 150.f, 0.f);
	FollowCamera->SetRelativeRotation(FRotator(0.f, -15.f, 0.f));

	
}

void ADefaultUnreal5Character::ESCDown()
{
	//UE_LOG(LogTemp, Warning, TEXT("pause"));

	if (AlphaEnding)
	{
		Resurrect();
		AlphaEnding = false;
		reverseEndScreen = true;
	}
	else if (!bNotDead && !bFallDeath)
	{
		Resurrect();
	}
	else if (mainPlayerController)
	{
		mainPlayerController->TogglePauseMenu();
	}
}

void ADefaultUnreal5Character::Unfreeze()
{
	GetCapsuleComponent()->SetCapsuleRadius(35);
	GetMesh()->bPauseAnims = false;
	bFrozen = false;
	GetMesh()->SetMaterial(0, regularMaterial);
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	bBeforeFreeze = false;
	bFreezing = false;
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	//allowrotation
//	UE_LOG(LogTemp, Warning, TEXT("Penis3"));
}


float ADefaultUnreal5Character::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//1 = attack, 2 = throw, 3 = pullback after parry and also kick, 4 = freeze, 5 = fall killzone, 6 = scream, 7 = bat, 8 = bear ending
	if(movementStatus != EMovementStatus::EMS_Dead && !bPullingAxe)
	{
		AAxe* damageAxe = Cast<AAxe>(DamageCauser);
		ABat* damageBat = Cast<ABat>(DamageCauser);

		if(Cast<AAxe>(DamageCauser))
			damageAxe = Cast<AAxe>(DamageCauser);
		else if(Cast<ABat>(DamageCauser))
			damageBat = Cast<ABat>(DamageCauser);
		
		FRotator kickerRotation;

		if(EventInstigator)
			kickerRotation = EventInstigator->GetControlRotation();

		//Different types of damage taken
		if (Damage == 8) //Bear Alpha Ending
		{
			AlphaEnding = true;
			Death();
			return 0;
		}
		else if (Damage == 5)//Fall death
		{
			bFallDeath = true;
			Death();
			return 0;
		}
		else if (Damage == 6) //Scream
		{
			if (!bStumble)
			{
				Stumbling();
				UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), kickedSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

				SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));
				return 0;
			}
			
		}
		else if (Damage == 7) //bat
		{

			if (!bFrozen)
			{
				//	UGameplayStatics::ApplyDamage(EventInstigator->GetPawn(), 3, GetController(), this, damageTypeClass);
				if (!bBatHit)
				{
					//	BatHit();
					UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), kickedSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

					SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));

					if ((health - 1) < 1)
					{
						health -= 1;
						Death();
					}
					else
					{
						health -= 1;
						BatHit();
					}
					return 0;
				}

			}
			else if (bFreezing)
			{
				Stumbling();
				return 0;
			}
			else if (damageBat) //bat hit while freezing
			{
				UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), parrySound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

				UGameplayStatics::ApplyDamage(EventInstigator->GetPawn(), 3, GetController(), this, damageTypeClass);
				//	Unfreeze();
				return 0;
			}
			
		}
		else if (Damage == 3)
		{
			 if (bFrozen)
			{
				Unfreeze();
				Stumbling();
				UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), kickedSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

				SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));
				return 0;
			}
			else
			 {
				 //	UGameplayStatics::ApplyDamage(EventInstigator->GetPawn(), 3, GetController(), this, damageTypeClass);
				 if (!bStumble)
				 {
					 Stumbling();
					 UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), kickedSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

					 SetActorRotation(FRotator(0.f, kickerRotation.Yaw - 180, 0.f));
					 return 0;
				 }
			 }
			//Pull Back
			
		}
		else if (Damage == 4)
		{
			if (!bFrozen && animInstance && bNotDead)
			{
				//	Stumbling();
			//	animInstance->Montage_Pause();
				
				AimOff();
				GetMesh()->bPauseAnims = true;
				bFrozen = true;
				FTimerHandle UnusedHandle;
				GetWorldTimerManager().SetTimer(UnusedHandle, this, &ADefaultUnreal5Character::Unfreeze, 2.f, false);
				return 0;
			}
		}
		else if (Damage == 1 || Damage == 2)
		{
			if (bFrozen && Damage != 2)
			{
				UGameplayStatics::ApplyDamage(EventInstigator->GetPawn(), 3, GetController(), this, damageTypeClass);
			//	UE_LOG(LogTemp, Warning, TEXT("Parry Executed"));

				UAudioComponent* sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), parrySound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 0.f, quietAttenuation, concurrency, false);

			//	Unfreeze();
				return 0;
			}
			else if (bFrozen && Damage == 2)
			{
				if(damageAxe)
					damageAxe->Drop();

				return 0;
			}
			if (bBeforeFreeze)
			{
				Stumbling();
				return 0;
			}
			else
			{
				Death();
				return 0;
			}
		}
	}
	return 0;
}