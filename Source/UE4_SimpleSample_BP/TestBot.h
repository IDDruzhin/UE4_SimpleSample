// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE4_SimpleSample_BP.h"
#include "TestPersone.h"
#include "TestAIController.h"
#include "TestBot.generated.h"

UCLASS()
class UE4_SIMPLESAMPLE_BP_API ATestBot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* R_Hand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* L_Hand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UAnimBlueprintGeneratedClass* DynamicBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCableComponent* R_Link;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCableComponent* L_Link;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		UBlackboardComponent* BlackBoard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TArray<ATargetPoint*> TargetPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		uint8 StartPatrolPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FVector BaseColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		FName BB_CurPatrolPointIndex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		FName BB_CurPatrolPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		FName BB_IsChasing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		FName BB_ChaseObject;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		FName BB_LastChasePos;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		FName BB_SeeChaseObject;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		FName BB_IsNavJumping;

	//Timers
	UPROPERTY()
		FTimerHandle LoseTargetTimer;
	UPROPERTY()
		FTimerHandle ChasingWaitTimer;
	UPROPERTY()
		FTimerHandle NavJumpTimer;
	//Timers functions
	UFUNCTION()
		void LoseTarget();
	UFUNCTION()
		void ChasingWait();
	UFUNCTION()
		void NavJumpWait();

	UFUNCTION(Category = "AI")
		void OnSeePawn(APawn* SomePawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "AI")
		void NavJump(FVector Destination, float Time = 1.0f, float Power = 500.0f);
	
};
