// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerCharacter.h"

// Sets default values
AVRPlayerCharacter::AVRPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitiateComponents();
	SetPlayerInitialStats();

}


// Called every frame
void AVRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculatePlayerStats();
}

// Called to bind functionality to input
void AVRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement Bindings

	// Left thumb y
	PlayerInputComponent->BindAxis(TEXT("MotionControllerThumbLeft_Y"), this, &AVRPlayerCharacter::MoveForward);

	// left thumb x
	PlayerInputComponent->BindAxis(TEXT("MotionControllerThumbLeft_X"), this, &AVRPlayerCharacter::MoveRight);

	// right thumb x
	PlayerInputComponent->BindAxis(TEXT("MotionControllerThumbRight_X"), this, &AVRPlayerCharacter::TurnAtRate);



	// Action Bindings

	// Pressed Left Stick to sprint
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AVRPlayerCharacter::isSprinting);

	// Released Left Stick to stop sprint
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AVRPlayerCharacter::isNotSprinting);

	// Pressed A to jump
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AVRPlayerCharacter::Jump);
}

void AVRPlayerCharacter::InitiateComponents()
{
	// Setting up our character components
	InitRootCameraHead();
	InitLeftHandandWatch();
	InitRightHand();
	InitBody();
}

void AVRPlayerCharacter::InitRootCameraHead()
{
	// Setting our root component
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	// Setting up camera 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	// Creating character head mesh
	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Camera);
	ConstructorHelpers::FObjectFinder<UStaticMesh> HeadMesh(TEXT("/Game/Meshes/VirtualReality/Meshes/GenericHMD")); // Finding our head mesh  
	Head->SetStaticMesh(HeadMesh.Object); // Setting head mesh
}

void AVRPlayerCharacter::InitLeftHandandWatch()
{
	// Setting left controller and hand mesh
	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	LeftController->SetupAttachment(VRRoot);
	LeftController->SetTrackingSource(EControllerHand::Left);
	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(LeftController);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> LeftHandMesh(TEXT("/Game/Meshes/VirtualReality/Mannequin/Character/Mesh/MannequinHand_Right")); // Finding our hand mesh  
	LeftHand->SetSkeletalMesh(LeftHandMesh.Object); // Setting hand mesh
	LeftHandScale = FVector(-1.0f, -1.0f, -1.0f);
	LeftHand->SetRelativeScale3D(LeftHandScale); // Inversing/transposing our hand mesh
	LeftHandPosition = FVector(-12.0f, 1.5f, -3.00f);
	LeftHandRotation = FRotator(0.0f, 180.0f, -90.0f);
	LeftHand->SetMobility(EComponentMobility::Movable);
	LeftHand->SetWorldLocationAndRotation(LeftHandPosition, LeftHandRotation); // Natural setting puts hands beyond controllers, and rotated incorrectly


	// Left Hand Grab Collision

	LeftHandGrabCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Left Hand Grab Collision"));
	LeftHandGrabCollision->SetupAttachment(LeftHand);
	LeftHandGrabCollision->InitSphereRadius(7); // Sphere Size Based On float
	LeftHandGrabCollisionLocation = FVector(11.0f, 0.0f, 0.0f);
	LeftHandGrabCollision->SetRelativeLocation(LeftHandGrabCollisionLocation);// Setting up the location of the Left hand grab relative to the left hand

	// Setting up watch for left hand
	Watch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Watch"));
	Watch->SetupAttachment(LeftHand);
	ConstructorHelpers::FObjectFinder<UStaticMesh> WatchMesh(TEXT("/Game/Meshes/Watch1")); // We can use this to make it so characters can choose their watch color????.  
	Watch->SetStaticMesh(WatchMesh.Object);
	WatchScale = FVector(0.1f, 0.1f, 0.1f); // our model was a little large
	Watch->SetRelativeScale3D(WatchScale);
	WatchRotation = FRotator(0.0f, 90.0f, 90.0f);
	WatchPosition = FVector(0.0f, -0.4f, -1.9f);
	Watch->SetMobility(EComponentMobility::Movable);
	Watch->SetWorldLocationAndRotation(WatchPosition, WatchRotation);
}

void AVRPlayerCharacter::InitRightHand()
{

	// setting right controller and hand mesh
	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightController->SetupAttachment(VRRoot);
	RightController->SetTrackingSource(EControllerHand::Right);
	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RightController);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> RightHandMesh(TEXT("/Game/Meshes/VirtualReality/Mannequin/Character/Mesh/MannequinHand_Right")); // Finding our hand mesh  
	RightHand->SetSkeletalMesh(RightHandMesh.Object); // Setting hand mesh
	RightHandPosition = FVector(-12.0f, 1.5f, -3.00f);
	RightHandRotation = FRotator(0.0f, 0.0f, 90.0f);
	RightHand->SetMobility(EComponentMobility::Movable);
	RightHand->SetWorldLocationAndRotation(RightHandPosition, RightHandRotation); // Natural setting puts hands beyond controllers, and rotated incorrectly


	//Right Hand Grab Collision

	RightHandGrabCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Right Hand Grab Collision"));
	RightHandGrabCollision->SetupAttachment(RightHand);
	RightHandGrabCollision->InitSphereRadius(7); // Sphere Size Based On float
	RightHandGrabCollisionLocation = FVector(11.0f, 0.0f, 0.0f);
	RightHandGrabCollision->SetRelativeLocation(RightHandGrabCollisionLocation);// Setting up the location of the Right hand grab relative to the Right hand

	//Right pointer finger collision

	RightPointerFingerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Right Pointer Finger Collision"));
	RightPointerFingerCollision->SetupAttachment(RightHand);
	RightPointerFingerCollision->InitSphereRadius(2); 
	RightPointerFingerCollisionLocation = FVector(21.33f, -3.6f, 0.6f);
	RightPointerFingerCollision->SetRelativeLocation(RightPointerFingerCollisionLocation);// Setting up the relative location of the Right hand finger collision
}

void AVRPlayerCharacter::InitBody()
{
	// Setting up our body mesh
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Head); // Attaching to head allows the body component to move along with head movement, up and down, side to side
	ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(TEXT("/Game/Meshes/1M_Cube")); // We can use this to make it so characters can choose their body.  
	Body->SetStaticMesh(BodyMesh.Object);
	BodyScale = FVector(0.33f, 0.43f, 0.75f);
	Body->SetRelativeScale3D(BodyScale);
	BodyLocation = FVector(-15.0f, 0.0f, -50.0f);
	Body->SetRelativeLocation(BodyLocation);


	// Creating toolbelt collision for detection
	ToolBeltCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Tool Belt Collision"));
	ToolBeltCollision->SetupAttachment(Body);
	ToolBeltCollision->InitSphereRadius(10); // Sphere Size Based On float
	ToolBeltLocation = FVector(30.0f, 0.0f, 0.0f);
	ToolBeltCollision->SetRelativeLocation(ToolBeltLocation);// Setting up the location of the toolbelt relative to the backpack

	// Creating backpack hand collision for overlap retrieval
	BackpackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Backpack Collision"));
	BackpackCollision->SetupAttachment(Body);
	BackpackCollision->InitSphereRadius(5);
}

void AVRPlayerCharacter::SetPlayerInitialStats()
{
	// Character Variables
	CurrentPlayerLevel = 1;

	PointsToLevelUp = (CurrentPlayerLevel * 1000);
	CurrentLevelUpPoints = 0;

	// setting players max health based upon their level
	MaxPlayerHealth = (CurrentPlayerLevel * 100);
	CurrentPlayerHealth = MaxPlayerHealth;

	// setting players max stamina based upon their level
	MaxPlayerStamina = ((CurrentPlayerLevel * 1.2f) * 100);
	CurrentPlayerStamina = MaxPlayerStamina;

	// setting players max Food based upon their level
	MaxPlayerFood = ((CurrentPlayerLevel * 1.2f) * 100);
	CurrentPlayerFood = MaxPlayerFood;

	// setting players max Water based upon their level
	MaxPlayerWater = ((CurrentPlayerLevel * 1.2f) * 100);
	CurrentPlayerWater = MaxPlayerWater;
}

void AVRPlayerCharacter::CalculatePlayerStats()
{
	// Stats Related

	// Kill player if 0 or below health
	if (CurrentPlayerHealth <= 0)
	{
		void QuitGame();
	}


	// Leveling up our character
	if (CurrentLevelUpPoints >= PointsToLevelUp)
	{
		SetPlayerLevel();
	}

	// Decreasing food over time
	if (CurrentPlayerFood >= 0)
	{
		CurrentPlayerFood -= 0.0001f;
	}

	// Decreasing Water over time
	if (CurrentPlayerWater >= 0)
	{
		CurrentPlayerWater -= 0.0001f;
	}

	// If water or food are less than 20% stamina goes down 
	if ((CurrentPlayerWater / MaxPlayerWater) <= 0.2f || (CurrentPlayerFood / MaxPlayerWater) <= 0.2f)
	{
		CurrentPlayerStamina -= 0.015f;
	}

	// if water or food are less than 0 Health goes down 
	if ((CurrentPlayerWater / MaxPlayerWater) <= 0 || (CurrentPlayerFood / MaxPlayerWater) <= 0)
	{
		CurrentPlayerHealth -= 0.015f;
	}



	if (bIsSprinting && CurrentPlayerStamina >= 0) // losing stamina, food, and water from sprinting
	{
		CurrentPlayerStamina -= 0.15f;
		CurrentPlayerFood -= 0.0015f;
		CurrentPlayerWater -= 0.0015f;
	}

	if (!bIsSprinting && CurrentPlayerStamina <= MaxPlayerStamina) // Regaining stamina when not sprinting
	{
		CurrentPlayerStamina += 0.05f;
	}
}

void AVRPlayerCharacter::SetPlayerLevel()
{
	// Leveling up character and resetting level up points
	CurrentPlayerLevel += 1;
	CurrentLevelUpPoints = 0;
}

void AVRPlayerCharacter::MoveForward(float Throttle) // Controls forward and reverse locomotion
{
	if (bIsSprinting && CurrentPlayerStamina >= 0)
	{
		AddMovementInput((Throttle) * Camera->GetForwardVector());
	}
	else
	{
		AddMovementInput((Throttle / 2)* Camera->GetForwardVector());
	}
}

void AVRPlayerCharacter::MoveRight(float Throttle) // Controls side to side movement
{
	if (bIsSprinting && CurrentPlayerStamina >= 0)
	{
		AddMovementInput((Throttle) * Camera->GetRightVector());

	}
	else
	{
		AddMovementInput((Throttle / 2) * Camera->GetRightVector());
	}
}

void AVRPlayerCharacter::TurnAtRate(float Rotation) //  Controls player rotation
{
	Sensitivity = 40.0f;
	float ComfortLevel = 0.6f;
	AddControllerYawInput((Rotation * Sensitivity * ComfortLevel)* GetWorld()->GetDeltaSeconds());
}

void AVRPlayerCharacter::isSprinting() // Sets sprint condition on
{
	bIsSprinting = true;
}

void AVRPlayerCharacter::isNotSprinting() // Sets sprint condition off
{
	bIsSprinting = false;
}

void AVRPlayerCharacter::Jump() // Sets jump to active
{
	ACharacter::Jump();
}
