// Fill out your copyright notice in the Description page of Project Settings.

#include "TestAIController.h"

void ATestAIController::BeginPlay()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> Tmp(TEXT("BehaviorTree'/Game/Characters/AI/Test_Bot_BT.Test_Bot_BT'"));
	RunBehaviorTree(Tmp.Object);
}



