// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XRMotionControllerBase.h"
#include "Components/StaticMeshComponent.h"
#include "Math/Rotator.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HeadMountedDisplayTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VRPlayerCharacter.generated.h"

UCLASS()
class VRCHARACTER_API AVRPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRPlayerCharacter();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	// Scene Components

	// Creating our VRRoot camera and head
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Head;

	// Setting up our body and relative collision components
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Body;

	UPROPERTY(BlueprintReadWrite)
	class USphereComponent* ToolBeltCollision;

	UPROPERTY(BlueprintReadWrite)
	class USphereComponent* BackpackCollision;

	// Setting up our right hand with collisions and watch to display stats
	UPROPERTY(BlueprintReadWrite)
	class UMotionControllerComponent* RightController;

	UPROPERTY(BlueprintReadWrite)
	class USkeletalMeshComponent* RightHand;

	UPROPERTY(BlueprintReadWrite)
	class USphereComponent* RightHandGrabCollision;

	UPROPERTY(BlueprintReadWrite)
	class USphereComponent* RightPointerFingerCollision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Watch;

	// Setting up our left hand with grab collision
	UPROPERTY(EditAnywhere)
	class UMotionControllerComponent* LeftController;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* LeftHand;

	UPROPERTY(BlueprintReadWrite)
	class USphereComponent* LeftHandGrabCollision;

private:
	// Configuration Parameters

	// Setting up controller input smooth rotation param
	UPROPERTY(EditAnywhere, Category = "Camera Rotation")
	float Sensitivity;

	// This is designed to set the location of the players body, and collisions
	UPROPERTY(EditAnywhere, Category = "Body")
	FVector BodyLocation;

	UPROPERTY(EditAnywhere, Category = "Body")
	FVector BodyScale;

	UPROPERTY(EditAnywhere, Category = "Tool Belt")
	FVector ToolBeltLocation;

	UPROPERTY(EditAnywhere, Category = "Backpack")
	FVector BackpackLocation;

	// Positioning Right hand, collisions, also manipulating watch location, scale and rotation
	UPROPERTY(EditAnywhere, Category = "Right Hand")
	FVector RightHandPosition;

	UPROPERTY(EditAnywhere, Category = "Right Hand")
	FRotator RightHandRotation;

	UPROPERTY(EditAnywhere, Category = "Right Hand")
	FVector RightHandGrabCollisionLocation;

	UPROPERTY(EditAnywhere, Category = "Right Hand")
	FVector RightPointerFingerCollisionLocation;

	UPROPERTY(EditAnywhere, Category = "Watch")
	FVector WatchScale;

	UPROPERTY(EditAnywhere, Category = "Watch")
	FVector WatchPosition;

	UPROPERTY(EditAnywhere, Category = "Watch")
	FRotator WatchRotation;

	// Positioning Left hand and grab collision
	UPROPERTY(EditAnywhere, Category = "Left Hand")
	FVector LeftHandLocation;

	UPROPERTY(EditAnywhere, Category = "Left Hand")
	FVector LeftHandScale;

	// Positioning Left hand, collisions, also manipulating watch location, scale and rotation
	UPROPERTY(EditAnywhere, Category = "Left Hand")
	FVector LeftHandPosition;

	UPROPERTY(EditAnywhere, Category = "Left Hand")
	FRotator LeftHandRotation;

	UPROPERTY(EditAnywhere, Category = "Left Hand")
	FVector LeftHandGrabCollisionLocation;


	// Setting our initial sprint state
	bool bIsSprinting = false;



public:

	// Player Stats

	// Character level
	UPROPERTY(BlueprintReadWrite, Category = "Player Level")
	float CurrentPlayerLevel;

	UPROPERTY(BlueprintReadWrite, Category = "Player Level")
	float PointsToLevelUp;

	UPROPERTY(BlueprintReadWrite, Category = "Player Level")
	float CurrentLevelUpPoints;


	// Character Health
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float MaxPlayerHealth;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float CurrentPlayerHealth;


	// Character Stamina
	UPROPERTY(BlueprintReadWrite, Category = "Stamina")
	float MaxPlayerStamina;

	UPROPERTY(BlueprintReadWrite, Category = "Stamina")
	float CurrentPlayerStamina;

	// Character Hunger
	UPROPERTY(BlueprintReadWrite, Category = "Food")
	float MaxPlayerFood;

	UPROPERTY(BlueprintReadWrite, Category = "Food")
	float CurrentPlayerFood;

	// Character thirst
	UPROPERTY(BlueprintReadWrite, Category = "Water")
	float MaxPlayerWater;

	UPROPERTY(BlueprintReadWrite, Category = "Water")
	float CurrentPlayerWater;


private:
	// Functions

	// Setting up our player character
	void InitiateComponents();

	// Initializing player character components
	
	// Setting initial information for root, camera and head
	void InitRootCameraHead();

	// Settining initial info for left hand and watch
	void InitLeftHandandWatch();

	// Setting up right hand as well as collisions
	void InitRightHand();

	// Setting up the body for the player and collisions
	void InitBody();


	// Set initial player stats
	void SetPlayerInitialStats();

	// Player Stat Functions tick based
	void CalculatePlayerStats();

	// Level Up Function
	void SetPlayerLevel();

	// Player Movement Functions
	void MoveForward(float Throttle);
	void MoveRight(float Throttle);
	void TurnAtRate(float Rotation);
	void isSprinting();
	void isNotSprinting();
	virtual void Jump();
	

	

	

	

};
