// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPersone.h"
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

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
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
	//CameraBoom->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 90.0f));
}


