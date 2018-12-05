// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBot.h"


// Sets default values
ATestBot::ATestBot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	JumpMaxCount = 2;
	JumpMaxHoldTime = 0.2f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 200;
	GetCharacterMovement()->AirControl = 0.3f;

	GetMesh()->SetRelativeLocation(FVector(2.322250f, -0.000035f, -87.684875f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	R_Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("R_Hand"));
	R_Hand->SetupAttachment(GetMesh());
	L_Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("L_Hand"));
	L_Hand->SetupAttachment(GetMesh());

	FAttachmentTransformRules AttachTransformRules(EAttachmentRule::KeepRelative, false);
	R_Link = CreateDefaultSubobject<UCableComponent>(TEXT("R_Link"));
	//R_Link->AttachTo(R_Hand, FName(TEXT("R_ForeArm_00Socket")));
	R_Link->AttachToComponent(R_Hand, AttachTransformRules, FName(TEXT("R_ForeArm_00Socket")));
	R_Link->bAttachStart = true;
	R_Link->bAttachEnd = true;
	R_Link->SetAttachEndTo(this, FName(TEXT("Mesh")), FName(TEXT("R_Hand")));
	R_Link->EndLocation = FVector(0.0f, -0.2f, 0.0f);
	R_Link->CableLength = 10.0f;
	R_Link->CableWidth = 1.0f;
	R_Link->NumSegments = 1;
	R_Link->NumSides = 6;
	R_Link->RelativeLocation = FVector(-0.0f, 0.131005f, -0.122708f);
	R_Link->RelativeRotation = FRotator(0.0f, 0.00001f, 0.0f);

	L_Link = CreateDefaultSubobject<UCableComponent>(TEXT("L_Link"));
	//L_Link->AttachToComponent(L_Hand, FName(TEXT("L_Arm_00")));
	L_Link->AttachToComponent(L_Hand, AttachTransformRules, FName(TEXT("L_Arm_00")));
	L_Link->bAttachStart = true;
	L_Link->bAttachEnd = true;
	L_Link->SetAttachEndTo(this, FName(TEXT("Mesh")), FName(TEXT("L_Arm_01")));
	L_Link->EndLocation = FVector(1.1f, 0.0f, 0.0f);
	L_Link->CableLength = 10.0f;
	L_Link->CableWidth = 1.0f;
	L_Link->NumSegments = 1;
	L_Link->NumSides = 6;
	L_Link->RelativeLocation = FVector(0.300961f, 0.080305f, 0.101101f);

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->HearingThreshold = 2600.0f;
	PawnSensing->SightRadius = 2000.0f;
	PawnSensing->SensingInterval = 0.1f;
	PawnSensing->SetPeripheralVisionAngle(70.0f);
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Tmp(TEXT("SkeletalMesh'/Game/Characters/Meshes/Test_Persone_Body.Test_Persone_Body'"));
		GetMesh()->SetSkeletalMesh(Tmp.Object);
		Tmp = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Characters/Meshes/Test_Persone_R_Hand.Test_Persone_R_Hand'"));
		R_Hand->SetSkeletalMesh(Tmp.Object);
		Tmp = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Characters/Meshes/Test_Persone_L_Hand.Test_Persone_L_Hand'"));
		L_Hand->SetSkeletalMesh(Tmp.Object);
	}
	{
		ConstructorHelpers::FObjectFinder<UAnimBlueprint> Tmp(TEXT("AnimBlueprint'/Game/Characters/Anims/Test_Persone_Anim_BP.Test_Persone_Anim_BP'"));
		DynamicBP = Tmp.Object->GetAnimBlueprintGeneratedClass();
		GetMesh()->SetAnimInstanceClass(DynamicBP);
		R_Hand->SetAnimInstanceClass(DynamicBP);
		L_Hand->SetAnimInstanceClass(DynamicBP);
	}
	{
		ConstructorHelpers::FObjectFinder<UMaterial> Tmp(TEXT("Material'/Game/Characters/Meshes/LinkMaterial.LinkMaterial'"));
		R_Link->SetMaterial(0, Tmp.Object);
		L_Link->SetMaterial(0, Tmp.Object);
	}
	{
		ConstructorHelpers::FObjectFinder<UMaterialInstance> Tmp(TEXT("MaterialInstanceConstant'/Game/Characters/Meshes/Test_Bot_Mat_Inst.Test_Bot_Mat_Inst'"));
		GetMesh()->SetMaterial(0, Tmp.Object);
		R_Hand->SetMaterial(0, Tmp.Object);
		L_Hand->SetMaterial(0, Tmp.Object);
	}

	StartPatrolPoint = 0;
	BaseColor = FVector(0.0f, 1.0f, 0.0f);
	BB_CurPatrolPointIndex = FName(TEXT("CurPatrolPointIndex"));
	BB_CurPatrolPoint = FName(TEXT("CurPatrolPoint"));
	BB_IsChasing = FName(TEXT("IsChasing"));
	BB_ChaseObject = FName(TEXT("ChaseObject"));
	BB_LastChasePos = FName(TEXT("LastChasePos"));
	BB_SeeChaseObject = FName(TEXT("SeeChaseObject"));
	BB_IsNavJumping = FName(TEXT("IsNavJumping"));

	AIControllerClass = ATestAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void ATestBot::BeginPlay()
{
	Super::BeginPlay();
	L_Hand->SetMasterPoseComponent(GetMesh());
	R_Hand->SetMasterPoseComponent(GetMesh());
	FName ColorParam = FName(TEXT("BaseColor"));
	GetMesh()->SetVectorParameterValueOnMaterials(ColorParam, BaseColor);
	L_Hand->SetVectorParameterValueOnMaterials(ColorParam, BaseColor);
	R_Hand->SetVectorParameterValueOnMaterials(ColorParam, BaseColor);
	BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(GetController());
	//BlackBoard->SetValueAsInt(BB_CurPatrolPointIndex, StartPatrolPoint);
	/*
	BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	BlackBoard->SetValueAsInt(BB_CurPatrolPointIndex, StartPatrolPoint);
	BlackBoard->SetValueAsObject(BB_CurPatrolPoint, TargetPoints[StartPatrolPoint]);
	BlackBoard->SetValueAsBool(BB_SeeChaseObject, false);
	BlackBoard->SetValueAsBool(BB_IsChasing, false);
	*/
}

void ATestBot::OnSeePawn(APawn * SomePawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("See PAWN"));
	ATestPersone* TPers = Cast<ATestPersone>(SomePawn);
	if (TPers)
	{
		BlackBoard->SetValueAsObject(BB_ChaseObject, TPers);
		BlackBoard->SetValueAsVector(BB_LastChasePos, TPers->GetActorLocation());
		BlackBoard->SetValueAsBool(BB_SeeChaseObject, true);
		GetWorldTimerManager().SetTimer(SeeTargetTimer, [this]() {
			BlackBoard->SetValueAsBool(BB_IsChasing, true);
			BlackBoard->SetValueAsBool(BB_SeeChaseObject, false);
			GetWorldTimerManager().SetTimer(ChasingTimer, [this]() { BlackBoard->SetValueAsBool(BB_IsChasing, false); }, 3.0f, false);
		}, 1.0f, false);
	}
}

void ATestBot::NavJump(FVector Destination, float Time, float Power)
{
	BlackBoard->SetValueAsBool(BB_IsNavJumping, true);
	FVector LaunchVelocity = (Destination - GetActorLocation()) / Time;
	LaunchVelocity.Z = Power;
	LaunchCharacter(LaunchVelocity, true, true);
	GetWorldTimerManager().SetTimer(NavJumpTimer, [this]() { BlackBoard->SetValueAsBool(BB_IsNavJumping, false); }, Time, false);
}

// Called every frame
void ATestBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensing->OnSeePawn.AddDynamic(this, &ATestBot::OnSeePawn);
}

