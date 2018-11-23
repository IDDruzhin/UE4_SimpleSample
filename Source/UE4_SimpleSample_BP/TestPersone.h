// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE4_SimpleSample_BP.h"
//#include "GameFramework/SpringArmComponent.h"
//#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
//#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "TestPersone.generated.h"

UENUM(BlueprintType)
enum class HandGrabStatus : uint8
{
	FREE UMETA(DisplayName = "Free"),
	FLYING_TO_GRAPPLE_POINT UMETA(DisplayName = "FlyingToGrapplePoint"),
	FLYING_BACK UMETA(DisplayName = "FlyingBack"),
	GRABBING UMETA(DisplayName = "Grabbing")
};

UCLASS()
class UE4_SIMPLESAMPLE_BP_API ATestPersone : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestPersone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnJumped_Implementation() override;

	//Input
	virtual void MoveForward(float v);
	virtual void MoveRight(float v);
	virtual void Turn(float v);
	virtual void LookUp(float v);
	virtual void CalculateVectors();
	virtual void JumpAction();
	virtual void JumpStopAction();
	virtual void IncreaseConnectionAction();

	virtual void WallRunActivate();
	virtual void WallRunDeactivate();

	void LineTraceWalls();
	void WallRun();

	UFUNCTION()
		void SetWallRunLocation();
	UFUNCTION()
		void SearchHook();

	///Components:
	UPROPERTY(EditAnywhere)
		USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* R_Hand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* L_Hand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimBlueprintGeneratedClass* DynamicBP;
			
	UPROPERTY(EditAnywhere)
		UCableComponent* R_Link;
	UPROPERTY(EditAnywhere)
		UCableComponent* L_Link;

	UPROPERTY(EditAnywhere)
		FVector Forward;
	UPROPERTY(EditAnywhere)
		FVector Right;

	///Timers:
	UPROPERTY(EditAnywhere)
		FTimerHandle JumpTimer;
	UPROPERTY(EditAnywhere)
		FTimerHandle WallRunTimer;

	///States:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		bool DoJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		bool HitWallForward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		bool HitWallRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		bool HitWallLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		bool WallRunActivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		bool IsWallRunning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		bool Ragdoll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		bool CanHook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		bool IsConnectionFixed;

	///Walls:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walls")
		FVector HitWallNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walls")
		FVector HitWallLocation;

	///Swing:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing")
		AActor* NearestGrappleHook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing")
		AActor* GrabbedGrappleHook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing")
		UMaterialInstanceDynamic* CurHookMatInst;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing")
		float MaxConnectionDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Swing")
		float CurConnectionDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing")
		float CurConnectionDelta;

	///Hands:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hands")
		HandGrabStatus RHandGrabStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hands")
		FVector RHandStartPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hands")
		FVector RHandCurPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hands")
		FVector RHandFinishPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hands")
		float RHandFlyTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hands")
		FVector GrapplePoint;

	///Misc:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
		bool WallRunFlag;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitProperties() override;

	
};
