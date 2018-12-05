// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UE4_SimpleSample_BP.h"
#include "TestAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE4_SIMPLESAMPLE_BP_API ATestAIController : public AAIController
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
