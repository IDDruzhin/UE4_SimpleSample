// Fill out your copyright notice in the Description page of Project Settings.

#include "GrappleHook.h"


// Sets default values
AGrappleHook::AGrappleHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Tmp(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
		Sphere->SetStaticMesh(Tmp.Object);
	}
	{
		ConstructorHelpers::FObjectFinder<UMaterialInstance> Tmp(TEXT("MaterialInstanceConstant'/Game/Levels/Actors/Hook_Material_Inst.Hook_Material_Inst'"));
		Sphere->SetMaterial(0, Tmp.Object);
	}
	//SetRootComponent(Sphere);
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetRelativeScale3D(FVector(0.493961f));
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere_Collision"));
	SphereCollision->SetupAttachment(Sphere);
	SphereCollision->SetRelativeScale3D(FVector(2.011221f));
	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AGrappleHook::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrappleHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UMaterialInstanceDynamic * AGrappleHook::GetMaterialInstance()
{
	return Sphere->CreateDynamicMaterialInstance(0);
}

