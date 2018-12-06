// Fill out your copyright notice in the Description page of Project Settings.

#include "TestAIController.h"

ATestAIController::ATestAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> Tmp(TEXT("BehaviorTree'/Game/Characters/AI/Test_Bot_BT.Test_Bot_BT'"));
	BT = Tmp.Object;
}

void ATestAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BT);
}



