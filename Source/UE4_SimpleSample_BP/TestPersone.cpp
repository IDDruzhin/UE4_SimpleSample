// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPersone.h"
//#include "CableComponent.h"
//#include "UE4_SimpleSample_BP.h"


// Sets default values
ATestPersone::ATestPersone()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	JumpMaxCount = 2;
	JumpMaxHoldTime = 0.2f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 200;
	GetCharacterMovement()->AirControl = 0.3f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	GetMesh()->SetRelativeLocation(FVector(2.322250f, -0.000035f, -87.684875f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	R_Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("R_Hand"));
	R_Hand->SetupAttachment(GetMesh());
	L_Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("L_Hand"));
	L_Hand->SetupAttachment(GetMesh());

	R_Link = CreateDefaultSubobject<UCableComponent>(TEXT("R_Link"));
	R_Link->AttachTo(R_Hand, FName(TEXT("R_ForeArm_00Socket")));
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
	L_Link->AttachTo(L_Hand, FName(TEXT("L_Arm_00")));
	L_Link->bAttachStart = true;
	L_Link->bAttachEnd = true;
	L_Link->SetAttachEndTo(this, FName(TEXT("Mesh")), FName(TEXT("L_Arm_01")));
	L_Link->EndLocation = FVector(1.1f, 0.0f, 0.0f);
	L_Link->CableLength = 10.0f;
	L_Link->CableWidth = 1.0f;
	L_Link->NumSegments = 1;
	L_Link->NumSides = 6;
	L_Link->RelativeLocation = FVector(0.300961f, 0.080305f, 0.101101f);

	//USkeletalMesh* Tmp = LoadObject<USkeletalMesh>(NULL, TEXT("RHandMesh"), TEXT("/Content/Characters/Meshes/Test_Persone_R_Hand.uasset"));
	//R_Hand->SetSkeletalMesh(Tmp);
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
		//GetMesh()->SetAnimInstanceClass(Tmp.Object->GetAnimBlueprintGeneratedClass());
		//R_Hand->SetAnimInstanceClass(Tmp.Object->GetAnimBlueprintGeneratedClass());
		//L_Hand->SetAnimInstanceClass(Tmp.Object->GetAnimBlueprintGeneratedClass());
	}
	{
		ConstructorHelpers::FObjectFinder<UMaterial> Tmp(TEXT("Material'/Game/Characters/Meshes/LinkMaterial.LinkMaterial'"));
		R_Link->SetMaterial(0, Tmp.Object);
		L_Link->SetMaterial(0, Tmp.Object);
	}
	
	//R_Hand->SetSkeletalMesh(Tmp.Object);
	//R_Hand->SetSkeletalMesh(LoadObject<USkeletalMesh>(NULL, TEXT("RHandMesh"), TEXT("SkeletalMesh'/Game/Characters/Meshes/Test_Persone_R_Hand.Test_Persone_R_Hand'")));
	//R_Hand->SetAnimInstanceClass(LoadObject<UAnimBlueprintGeneratedClass>(NULL, TEXT("RHandAnim"), TEXT("AnimBlueprint'/Game/Characters/Anims/Test_Persone_Anim_BP.Test_Persone_Anim_BP'")));

	//R_Link = CreateDefaultSubobject<UCableComponent>(TEXT("R_Link"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	//GetWorld()->GetTimerManager().SetTimer(WallRunTimer, this, &ATestPersone::SetWallRunLocation, 1.0f, true);
	//GetWorldTimerManager().SetTimer(WallRunTimer, &ATestPersone::SetWallRunLocation, 1.0f, true);
	//GetWorldTimerManager().PauseTimer(WallRunTimer);
}

// Called when the game starts or when spawned
void ATestPersone::BeginPlay()
{
	Super::BeginPlay();
	//GetWorldTimerManager().SetTimer(WallRunTimer, this, &ATestPersone::SetWallRunLocation, 1.0f, true);
	//GetWorldTimerManager().PauseTimer(WallRunTimer);
	//FTimerDelegate TimerD;
	//GetWorldTimerManager().SetTimer(WallRunTimer, this, &ATestPersone::SetWallRunLocation, 1.0f, true);
	/*
	TimerD.BindUFunction(this, FName("TestFoo"), 10.0f);
	GetWorld()->GetTimerManager().SetTimer(JumpTimer, TimerD, 1.0f, true);
	TimerD.BindUFunction(this, FName("TestFoo"), 200.0f);
	GetWorld()->GetTimerManager().SetTimer(FlyTimer, TimerD, 2.0f, true);
	*/
	//TestFoo(TEXT("START"));
	
}

void ATestPersone::OnJumped_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::FromInt((int)(DoJump)));
	DoJump = true;
	GetWorldTimerManager().SetTimer(JumpTimer, [this]() { DoJump = false; }, 0.1f, false);
}

void ATestPersone::MoveForward(float v)
{
	AddMovementInput(Forward, v);
	UE_LOG(LogTemp, Warning, TEXT("Forward"));
}

void ATestPersone::MoveRight(float v)
{
	AddMovementInput(Right, v);
}

void ATestPersone::Turn(float v)
{
	AddControllerYawInput(v);
}

void ATestPersone::LookUp(float v)
{
	AddControllerPitchInput(v);
}

void ATestPersone::CalculateVectors()
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;
	Forward = UKismetMathLibrary::GetForwardVector(ControlRot);
	Right = UKismetMathLibrary::GetRightVector(ControlRot);
}

void ATestPersone::JumpAction()
{
	CurConnectionDelta = 10.0f;
	if (IsWallRunning)
	{
		FVector LaunchVelocity = HitWallNormal * 600.0f;
		LaunchVelocity.Z = 250;
		LaunchCharacter(LaunchVelocity, false, false);
		IsWallRunning = false;
		return;
	}
	else
	{
		if (GetCharacterMovement()->IsFalling())
		{
			if (HitWallForward || HitWallLeft || HitWallRight)
			{
				FVector LaunchVelocity = HitWallNormal * 600.0f;
				LaunchVelocity.Z = 250;
				LaunchCharacter(LaunchVelocity, false, false);
				return;
			}
		}
	}
	Jump();
}

void ATestPersone::JumpStopAction()
{
	StopJumping();
	CurConnectionDelta = 0.0f;
}

void ATestPersone::IncreaseConnectionAction()
{
}

void ATestPersone::WallRunActivate()
{
	WallRunActivity = true;
}

void ATestPersone::WallRunDeactivate()
{
	WallRunActivity = false;
}

void ATestPersone::LineTraceWalls()
{
	HitWallLeft = false;
	HitWallRight = false;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 70.0f;
	FHitResult Hit;
	HitWallForward = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ECC_WorldStatic);
	if (HitWallForward)
	{
		HitWallNormal = Hit.Normal;
		HitWallLocation = Hit.Location;
	}
	Start = Start - GetActorRightVector() * 70.0f;
	End = GetActorLocation() + GetActorRightVector() * 70.0f;
	if (GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ECC_WorldStatic))
	{
		HitWallLocation = Hit.Location;
		if (Hit.Time < 0.5f)
		{
			HitWallLeft = true;
			HitWallNormal = -Hit.Normal;
		}
		else
		{
			HitWallRight = true;
			HitWallNormal = Hit.Normal;
		}
	}
}


void ATestPersone::WallRun()
{
	if (WallRunActivity && (HitWallLeft || HitWallRight))
	{
		if (!WallRunFlag)
		{
			IsWallRunning = true;
			GetCharacterMovement()->GravityScale = 0.0f;
			SetWallRunLocation();
			GetWorldTimerManager().SetTimer(WallRunTimer, this, &ATestPersone::SetWallRunLocation, 1.0f, true);
			WallRunFlag = true;
		}
	}
	else
	{
		if (WallRunFlag)
		{
			IsWallRunning = false;
			GetCharacterMovement()->GravityScale = 1.0f;
			GetWorld()->GetTimerManager().PauseTimer(WallRunTimer);
			WallRunFlag = false;
		}
	}
	if (IsWallRunning)
	{
		if (HitWallLeft)
		{
			SetActorRotation(UKismetMathLibrary::MakeRotFromYX(HitWallNormal, GetActorForwardVector()));
		}
		else if (HitWallRight)
		{
			SetActorRotation(UKismetMathLibrary::MakeRotFromYX(-HitWallNormal, GetActorForwardVector()));
		}
		LaunchCharacter(GetActorForwardVector()*900.0f, true, true);
	}
}

void ATestPersone::SetWallRunLocation()
{
	SetActorLocation(HitWallLocation + HitWallNormal * 40.0f);
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Correct"));
}

void ATestPersone::SearchHook()
{
	CanHook = false;
	//UKismetSystemLibrary::SphereTraceMulti(GetWorld())
}

// Called every frame
void ATestPersone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateVectors();
	LineTraceWalls();
	WallRun();
}

// Called to bind functionality to input
void ATestPersone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATestPersone::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATestPersone::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ATestPersone::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ATestPersone::LookUp);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATestPersone::JumpAction);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATestPersone::JumpStopAction);
	PlayerInputComponent->BindAction("WallRun", IE_Pressed, this, &ATestPersone::WallRunActivate);
	PlayerInputComponent->BindAction("WallRun", IE_Released, this, &ATestPersone::WallRunDeactivate);
}

void ATestPersone::PostInitProperties()
{
	Super::PostInitProperties();
	//GetMesh()->SetAnimInstanceClass(DynamicBP);
	//CameraBoom->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 90.0f));
}


