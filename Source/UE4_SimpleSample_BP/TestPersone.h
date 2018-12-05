// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE4_SimpleSample_BP.h"
#include "GrappleHook.h"
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
	virtual void WallRunActivate();
	virtual void WallRunDeactivate();
	virtual void ActionPressed();
	virtual void ActionReleased();
	virtual void FixConnection();
	virtual void UnfixConnection();
	virtual void IncreaseConnectionPressed();
	virtual void IncreaseConnectionReleased();
	virtual void ToggleRagdoll();

	//Tick
	void LineTraceWalls();
	void WallRun();
	void UpdateGrapplePoint();
	void Swing();

	UFUNCTION()
		void SetWallRunLocation();
	UFUNCTION()
		void SearchHook();

	///Components:
	UPROPERTY(EditDefaultsOnly)
		USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly)
		UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* R_Hand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* L_Hand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimBlueprintGeneratedClass* DynamicBP;
	UPROPERTY(EditAnywhere)
		UPhysicsAsset* R_Hand_PA;
	UPROPERTY(EditAnywhere)
		UPhysicsAsset* L_Hand_PA;
	UPROPERTY(EditAnywhere)
		UCableComponent* R_Link;
	UPROPERTY(EditAnywhere)
		UCableComponent* L_Link;

	UPROPERTY()
		FVector Forward;
	UPROPERTY()
		FVector Right;

	///Timers:
	UPROPERTY()
		FTimerHandle JumpTimer;
	UPROPERTY()
		FTimerHandle WallRunTimer;
	UPROPERTY()
		FTimerHandle SearchHookTimer;
	UPROPERTY()
		FTimerHandle RHandFlyToPointTimer;
	UPROPERTY()
		FTimerHandle RHandFlyBackTimer;

	///States:
	UPROPERTY(BlueprintReadWrite, Category = "States")
		bool DoJump;
	UPROPERTY(BlueprintReadWrite, Category = "States")
		bool HitWallForward;
	UPROPERTY(BlueprintReadWrite, Category = "States")
		bool HitWallRight;
	UPROPERTY(BlueprintReadWrite, Category = "States")
		bool HitWallLeft;
	UPROPERTY(BlueprintReadWrite, Category = "States")
		bool WallRunActivity;
	UPROPERTY(BlueprintReadWrite, Category = "States")
		bool IsWallRunning;
	UPROPERTY(BlueprintReadWrite, Category = "States")
		bool Ragdoll;
	UPROPERTY(BlueprintReadWrite, Category = "States")
		bool CanHook;
	UPROPERTY(BlueprintReadWrite, Category = "States")
		bool IsConnectionFixed;

	///Walls:
	UPROPERTY(BlueprintReadWrite, Category = "Walls")
		FVector HitWallNormal;
	UPROPERTY(BlueprintReadWrite, Category = "Walls")
		FVector HitWallLocation;

	///Swing:
	UPROPERTY(BlueprintReadWrite, Category = "Swing")
		AGrappleHook* NearestGrappleHook;
	UPROPERTY(BlueprintReadWrite, Category = "Swing")
		AGrappleHook* GrabbedGrappleHook;
	UPROPERTY(BlueprintReadWrite, Category = "Swing")
		UMaterialInstanceDynamic* CurHookMatInst;
	UPROPERTY(BlueprintReadWrite, Category = "Swing")
		float MaxConnectionDist;
	UPROPERTY(BlueprintReadWrite, Category="Swing")
		float CurConnectionDist;
	UPROPERTY(BlueprintReadWrite, Category = "Swing")
		float CurConnectionDelta;

	///Hands:
	UPROPERTY(BlueprintReadWrite, Category = "Hands")
		HandGrabStatus RHandGrabStatus;
	UPROPERTY(BlueprintReadWrite, Category = "Hands")
		FVector RHandStartPos;
	UPROPERTY(BlueprintReadWrite, Category = "Hands")
		FVector RHandCurPos;
	UPROPERTY(BlueprintReadWrite, Category = "Hands")
		FVector GrapplePoint;

	///Misc:
	UPROPERTY(BlueprintReadWrite, Category = "Misc")
		bool WallRunFlag;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Destroyed() override;
	
};
