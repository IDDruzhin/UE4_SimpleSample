// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPersone.h"

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

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	GetMesh()->SetRelativeLocation(FVector(2.322250f, -0.000035f, -87.684875f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	R_Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("R_Hand"));
	R_Hand->SetupAttachment(GetMesh());
	L_Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("L_Hand"));
	L_Hand->SetupAttachment(GetMesh());

	FAttachmentTransformRules AttachTransformRules(EAttachmentRule::KeepRelative, false);
	R_Link = CreateDefaultSubobject<UCableComponent>(TEXT("R_Link"));
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
		ConstructorHelpers::FObjectFinder<UMaterialInstance> Tmp(TEXT("MaterialInstanceConstant'/Game/Characters/Meshes/LinkMaterial_Inst.LinkMaterial_Inst'"));
		R_Link->SetMaterial(0, Tmp.Object);
		L_Link->SetMaterial(0, Tmp.Object);
	}
	{
		ConstructorHelpers::FObjectFinder<UPhysicsAsset> Tmp(TEXT("PhysicsAsset'/Game/Characters/Meshes/Test_Persone_L_Hand_PhysicsAsset.Test_Persone_L_Hand_PhysicsAsset'"));
		L_Hand_PA = Tmp.Object;
		Tmp = ConstructorHelpers::FObjectFinder<UPhysicsAsset>(TEXT("PhysicsAsset'/Game/Characters/Meshes/Test_Persone_R_Hand_PhysicsAsset.Test_Persone_R_Hand_PhysicsAsset'"));
		R_Hand_PA = Tmp.Object;
	}

	GetMesh()->SetCollisionProfileName(FName(TEXT("Pawn")));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	L_Hand->SetCollisionProfileName(FName(TEXT("Pawn")));
	L_Hand->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	R_Hand->SetCollisionProfileName(FName(TEXT("Pawn")));
	R_Hand->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	NearestGrappleHook = nullptr;
	GrabbedGrappleHook = nullptr;
	CurHookMatInst = nullptr;
	MaxConnectionDist = 2000.0f;
	CurConnectionDist = 0.0f;
	CurConnectionDelta = 0.0f;
	RHandGrabStatus = HandGrabStatus::FREE;
	GrapplePoint = FVector(0.0f);
	LinkFreeColor = FVector(1.0f,0.0f,0.03f);
	LinkFreeOpacity = 0.5f;
	LinkFreeEmissive = 0.0f;
	LinkFixedColor = FVector(0.0f, 0.7f, 1.0f);
	LinkFixedOpacity = 0.9f;
	LinkFixedEmissive = 10.0f;
	R_Link->SetVectorParameterValueOnMaterials(FName(TEXT("Color")), LinkFreeColor);
	R_Link->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), LinkFreeOpacity);
	R_Link->SetScalarParameterValueOnMaterials(FName(TEXT("Emissive")), LinkFreeEmissive);
	L_Link->SetVectorParameterValueOnMaterials(FName(TEXT("Color")), LinkFreeColor);
	L_Link->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), LinkFreeOpacity);
	L_Link->SetScalarParameterValueOnMaterials(FName(TEXT("Emissive")), LinkFreeEmissive);
}

// Called when the game starts or when spawned
void ATestPersone::BeginPlay()
{
	Super::BeginPlay();
	L_Hand->SetMasterPoseComponent(GetMesh());
	R_Hand->SetMasterPoseComponent(GetMesh());
	GetWorldTimerManager().SetTimer(SearchHookTimer, this, &ATestPersone::SearchHook, 0.1f, true);	
}

void ATestPersone::OnJumped_Implementation()
{
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
	CurConnectionDelta = -10.0f;
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

void ATestPersone::WallRunActivate()
{
	WallRunActivity = true;
}

void ATestPersone::WallRunDeactivate()
{
	WallRunActivity = false;
}

void ATestPersone::ActionPressed()
{
	if (CanHook)
	{
		GetWorldTimerManager().ClearTimer(RHandFlyBackTimer);
		GrabbedGrappleHook = NearestGrappleHook;
		if (RHandGrabStatus == HandGrabStatus::FREE)
		{
			RHandStartPos = GetMesh()->GetSocketLocation(FName(TEXT("R_ForeArm_00Socket")));
			RHandCurPos = RHandStartPos;
		}
		else
		{
			RHandStartPos = RHandCurPos;
		}
		RHandGrabStatus = HandGrabStatus::FLYING_TO_GRAPPLE_POINT;
		CurConnectionDist = FVector::Distance(GetActorLocation(), GrabbedGrappleHook->GetActorLocation());
		CurConnectionDist = UKismetMathLibrary::FClamp(CurConnectionDist, 100.0f, MaxConnectionDist);
		float TimerRate = 1.5f * CurConnectionDist / MaxConnectionDist;
		GetWorldTimerManager().SetTimer(RHandFlyToPointTimer, [this]() { RHandGrabStatus = HandGrabStatus::GRABBING; }, TimerRate, false);
	}
}

void ATestPersone::ActionReleased()
{
	if (RHandGrabStatus == HandGrabStatus::FLYING_TO_GRAPPLE_POINT || RHandGrabStatus == HandGrabStatus::GRABBING)
	{
		GetWorldTimerManager().ClearTimer(RHandFlyToPointTimer);
		RHandGrabStatus = HandGrabStatus::FLYING_BACK;
		RHandStartPos = RHandCurPos;
		float TimerRate = 1.5f * FVector::Distance(RHandCurPos, GetMesh()->GetSocketLocation(FName(TEXT("R_ForeArm_00Socket")))) / MaxConnectionDist;
		GetWorldTimerManager().SetTimer(RHandFlyBackTimer, [this]() { RHandGrabStatus = HandGrabStatus::FREE; }, TimerRate, false);
	}
	
}

void ATestPersone::FixConnection()
{
	IsConnectionFixed = true;
}

void ATestPersone::UnfixConnection()
{
	IsConnectionFixed = false;
}

void ATestPersone::IncreaseConnectionPressed()
{
	CurConnectionDelta = 10.0f;
}

void ATestPersone::IncreaseConnectionReleased()
{
	CurConnectionDelta = 0.0f;
}

void ATestPersone::ToggleRagdoll()
{
	if (Ragdoll)
	{
		GetWorld()->GetFirstPlayerController()->Possess(GetWorld()->SpawnActor<ATestPersone>(GetActorLocation(),GetActorRotation()));
	}
	else
	{
		L_Hand->SetMasterPoseComponent(nullptr);
		R_Hand->SetMasterPoseComponent(nullptr);
		L_Link->ToggleVisibility();
		R_Link->ToggleVisibility();
		{
			L_Hand->SetPhysicsAsset(L_Hand_PA, true);
			R_Hand->SetPhysicsAsset(R_Hand_PA, true);
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetSimulatePhysics(true);
		L_Hand->SetSimulatePhysics(true);
		R_Hand->SetSimulatePhysics(true);
		GetCharacterMovement()->Deactivate();
		Ragdoll = true;
	}
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

void ATestPersone::UpdateGrapplePoint()
{
	if (RHandGrabStatus != HandGrabStatus::FREE)
	{
		GrapplePoint = GrabbedGrappleHook->GetActorLocation();
	}
	else
	{
		if (CanHook)
		{
			GrapplePoint = UKismetMathLibrary::VInterpTo(GrapplePoint, NearestGrappleHook->GetActorLocation(), GetWorld()->GetDeltaSeconds(), 5.0f);
		}
		else
		{
			GrapplePoint = UKismetMathLibrary::VInterpTo(GrapplePoint, GetMesh()->GetSocketLocation(FName(TEXT("StraightLookSocket"))), GetWorld()->GetDeltaSeconds(), 5.0f);
		}
	}
}

void ATestPersone::Swing()
{
	if (RHandGrabStatus == HandGrabStatus::FREE)
	{
		return;
	}
	if (RHandGrabStatus == HandGrabStatus::GRABBING)
	{
		if (IsConnectionFixed && GetMovementComponent()->IsFalling())
		{
			CurConnectionDist = UKismetMathLibrary::FClamp(CurConnectionDist + CurConnectionDelta, 100.0f, MaxConnectionDist);
		}
	}
	FVector ActorToHookV = GrabbedGrappleHook->GetActorLocation() - GetActorLocation();
	if (!(IsConnectionFixed && RHandGrabStatus == HandGrabStatus::GRABBING))
	{
		CurConnectionDist = UKismetMathLibrary::FClamp(ActorToHookV.Size(), 100.0f, MaxConnectionDist);
	}
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, GrabbedGrappleHook->GetActorLocation(), GetMesh()->GetSocketLocation(FName(TEXT("R_ForeArm_00Socket"))), ECollisionChannel::ECC_Visibility))
	{
		GetCharacterMovement()->Velocity = ((GetCharacterMovement()->Velocity).Size() + 20.0f) * Hit.Normal;
	}
	if ((RHandGrabStatus == HandGrabStatus::GRABBING) && (CurConnectionDist == MaxConnectionDist || IsConnectionFixed))
	{
		if (GetMovementComponent()->IsFalling())
		{
			GetCharacterMovement()->AddForce(ActorToHookV.GetSafeNormal() * (FVector::DotProduct(ActorToHookV, GetVelocity())) * -2.0f);
		}
		FHitResult SweepHit;
		SetActorLocation(GrabbedGrappleHook->GetActorLocation() - ActorToHookV.GetSafeNormal() * CurConnectionDist, true, &SweepHit);
		if (SweepHit.bBlockingHit)
		{
			GetCharacterMovement()->AddForce(SweepHit.Normal * 1000000.0f);
			CurConnectionDist = (GrabbedGrappleHook->GetActorLocation() - GetActorLocation()).Size();
		}
		if (!IsLinkMatChanged)
		{
			R_Link->SetVectorParameterValueOnMaterials(FName(TEXT("Color")), LinkFixedColor);
			R_Link->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), LinkFixedOpacity);
			R_Link->SetScalarParameterValueOnMaterials(FName(TEXT("Emissive")), LinkFixedEmissive);
			IsLinkMatChanged = true;
		}
	}
	else
	{
		if (IsLinkMatChanged)
		{
			R_Link->SetVectorParameterValueOnMaterials(FName(TEXT("Color")), LinkFreeColor);
			R_Link->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), LinkFreeOpacity);
			R_Link->SetScalarParameterValueOnMaterials(FName(TEXT("Emissive")), LinkFreeEmissive);
			IsLinkMatChanged = false;
		}
	}
}

void ATestPersone::CorrectFloor()
{
	if (!(GetMovementComponent()->IsFalling() || DoJump))
	{
		FVector End = GetActorLocation();
		End.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 1.2f;
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByObjectType(Hit, GetActorLocation(), End, ECC_WorldDynamic))
		{
			FVector CorrectLoc = Hit.ImpactPoint;
			CorrectLoc.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			SetActorLocation(CorrectLoc);
		}
	}
}

void ATestPersone::SetWallRunLocation()
{
	SetActorLocation(HitWallLocation + HitWallNormal * 38.0f);
}

void ATestPersone::SearchHook()
{
	CanHook = false;
	FVector Center = GetActorLocation() + GetActorForwardVector() * 1000.0f;
	TArray<FHitResult> HitResults;
	if (GetWorld()->SweepMultiByChannel(HitResults, Center, Center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(1000.0f)))
	{	
		TArray<TTuple<float, uint8>> HitResDistances;
		for (int i = 0; i < HitResults.Num(); i++)
		{
			HitResDistances.Emplace(FVector::DistSquared(HitResults[i].GetActor()->GetActorLocation(), GetActorLocation()), i);
		}
		HitResDistances.Sort([](const TTuple<float, uint8> &A, const TTuple<float, uint8> &B) {
			return ((A.Get<0>()) < (B.Get<0>()));
		});
		FHitResult LineHit;
		AActor* CurCrappleHook;
		for (int i = 0; i < HitResDistances.Num(); i++)
		{
			CurCrappleHook = Cast<AGrappleHook>(HitResults[HitResDistances[i].Get<1>()].GetActor());
			if (CurCrappleHook == nullptr)
			{
				continue;
			}
			if (!(GetWorld()->LineTraceSingleByChannel(LineHit, GetActorLocation(), CurCrappleHook->GetActorLocation(), ECollisionChannel::ECC_Visibility)))
			{
				CanHook = true;
				if (NearestGrappleHook != CurCrappleHook)
				{
					if (CurHookMatInst != nullptr)
					{
						CurHookMatInst->SetVectorParameterValue(FName(TEXT("Color")), FVector(0.0f, 0.0f, 1.0f));
						CurHookMatInst->SetScalarParameterValue(FName(TEXT("Emissive")), 0.0f);
					}
					NearestGrappleHook = Cast<AGrappleHook>(CurCrappleHook);
					if (NearestGrappleHook != nullptr)
					{
						CurHookMatInst = NearestGrappleHook->GetMaterialInstance();
						CurHookMatInst->SetVectorParameterValue(FName(TEXT("Color")), FVector(0.0f, 1.0f, 0.0f));
						CurHookMatInst->SetScalarParameterValue(FName(TEXT("Emissive")), 1000.0f);
					}
				}
				break;
			}
		}
	}
	if (!CanHook)
	{
		if (CurHookMatInst != nullptr)
		{
			CurHookMatInst->SetVectorParameterValue(FName(TEXT("Color")), FVector(0.0f, 0.0f, 1.0f));
			CurHookMatInst->SetScalarParameterValue(FName(TEXT("Emissive")), 0.0f);
			CurHookMatInst = nullptr;
		}
		NearestGrappleHook = nullptr;
	}
}

// Called every frame
void ATestPersone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateVectors();
	LineTraceWalls();
	WallRun();
	UpdateGrapplePoint();
	Swing();
	CorrectFloor();

	///Timers:
	if (GetWorldTimerManager().IsTimerActive(RHandFlyToPointTimer))
	{
		RHandCurPos = UKismetMathLibrary::VLerp(RHandStartPos, GrabbedGrappleHook->GetActorLocation(), GetWorldTimerManager().GetTimerElapsed(RHandFlyToPointTimer) / GetWorldTimerManager().GetTimerRate(RHandFlyToPointTimer));
	}
	else if (GetWorldTimerManager().IsTimerActive(RHandFlyBackTimer))
	{
		RHandCurPos = UKismetMathLibrary::VLerp(RHandStartPos, GetMesh()->GetSocketLocation(FName(TEXT("R_ForeArm_00Socket"))), GetWorldTimerManager().GetTimerElapsed(RHandFlyBackTimer) / GetWorldTimerManager().GetTimerRate(RHandFlyBackTimer));
	}
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
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ATestPersone::ActionPressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &ATestPersone::ActionReleased);
	PlayerInputComponent->BindAction("FixConnection", IE_Pressed, this, &ATestPersone::FixConnection);
	PlayerInputComponent->BindAction("FixConnection", IE_Released, this, &ATestPersone::UnfixConnection);
	PlayerInputComponent->BindAction("IncreaseConnection", IE_Pressed, this, &ATestPersone::IncreaseConnectionPressed);
	PlayerInputComponent->BindAction("IncreaseConnection", IE_Released, this, &ATestPersone::IncreaseConnectionReleased);
	PlayerInputComponent->BindAction("Ragdoll", IE_Pressed, this, &ATestPersone::ToggleRagdoll);
}

void ATestPersone::Destroyed()
{
	if (GetWorld())
	{
		if (GetWorld()->GetFirstPlayerController())
		{
			if (GetWorld()->GetFirstPlayerController()->GetPawn() == this)
			{
				UGameplayStatics::OpenLevel(this, FName(*(GetWorld()->GetName())));
			}
		}
	}
	Super::Destroyed();
}


