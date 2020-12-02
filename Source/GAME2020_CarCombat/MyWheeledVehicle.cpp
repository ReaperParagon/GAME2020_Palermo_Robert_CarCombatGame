// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWheeledVehicle.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"

static const FName NAME_SteeringInput("Steer");
static const FName NAME_ThrottleInput("Throttle");

AMyWheeledVehicle::AMyWheeledVehicle()
{
	// UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	/* Wheeled Vehicle Stuff
	// Adjust the tire load
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;

	// Torque setup
	Vehicle4W->MaxEngineRPM = 9500.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(2150.0f, 700.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(7000.0f, 600.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(9530.0f, 400.0f);

	// Adjust the steering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(25.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(60.0f, 0.6f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.5f);

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65;

	// Automatic Gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;
	*/

	// Creating the Main Mesh for the Vehicle
	MeshActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshActor"));
	RootComponent = MeshActor;

	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(MeshActor);
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->bUsePawnControlRotation = true;

	// Create the chase camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.0f;

	// Health
	PlayerHealthMax = 100.0f;
	PlayerHealth = PlayerHealthMax;

	// Defaults
	TurretCount = 12;
	TimeRemaining = 180.0f;
	OnGround = 0;

	ThrottleVal = SteerVal = 0.0f;

	ThrottlePower = 16000.0f;
	TurnPower = 0.1f;
	BulletTimer = 0;
	TimerMax = 1.0f;

	BulletSpawn = FVector(10.0f, 0.0f, 150.0f);
}

void AMyWheeledVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IncrementTimeRemaining(-DeltaTime);

	HandleWheels();

	BulletTimer += DeltaTime;
	if (BulletTimer > TimerMax)
		BulletTimer = TimerMax;

	// If No wheels are on the ground
	if (OnGround == 0)
	{
		MeshActor->SetLinearDamping(0.1f);
	}
	else
	{
		MeshActor->SetLinearDamping(1.5f);

		if (ThrottleVal == 0)
			MeshActor->SetPhysicsLinearVelocity(MeshActor->GetPhysicsLinearVelocity() * 0.95f);

		if (SteerVal == 0)
			MeshActor->SetPhysicsAngularVelocity(MeshActor->GetPhysicsAngularVelocity() * 0.95f);
	}
	
	// Positioning Bullet Spawn Point
	BulletSpawn = 10.0f * MeshActor->GetForwardVector() + 150.0f * MeshActor->GetUpVector();

	// UpdateInAirControl(DeltaTime);
}

void AMyWheeledVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(NAME_ThrottleInput, this, &AMyWheeledVehicle::ApplyThrottle);
	PlayerInputComponent->BindAxis(NAME_SteeringInput, this, &AMyWheeledVehicle::ApplySteering);
	PlayerInputComponent->BindAxis("LookVertical", this, &AMyWheeledVehicle::LookVert);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AMyWheeledVehicle::LookHoriz);
	
	// PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AMyWheeledVehicle::OnHandBrakePressed);
	// PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AMyWheeledVehicle::OnHandBrakeReleased);
}

void AMyWheeledVehicle::ApplyThrottle(float val)
{
	ThrottleVal = val;
	if (OnGround != 0)
	{
		MeshActor->AddImpulse(((MeshActor->GetForwardVector() * ThrottlePower) - (MeshActor->GetUpVector() * 2000.0f)) * val);
	}
}

void AMyWheeledVehicle::ApplySteering(float val)
{
	SteerVal = val;
	if (OnGround != 0)
	{
		float TurningFactor = (TurnPower * (MeshActor->GetPhysicsLinearVelocity()).Size());
		MeshActor->AddTorqueInDegrees((MeshActor->GetUpVector() * TurningFactor) * ThrottleVal * val, FName(NAME_None), true);
		// GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString::Printf(TEXT("Steering Value: %f"), (ThrottleVal * SteerVal)));
	}
}

void AMyWheeledVehicle::LookVert(float val)
{
	if (val != 0.0f)
	{
		AddControllerPitchInput(val);
	}
}

void AMyWheeledVehicle::LookHoriz(float val)
{
	if (val != 0.0f)
	{
		AddControllerYawInput(val);
	}
}

void AMyWheeledVehicle::OnHandBrakePressed()
{
	// GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AMyWheeledVehicle::OnHandBrakeReleased()
{
	// GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AMyWheeledVehicle::UpdateInAirControl(float DeltaTime)
{
	/*
	if (UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement()))
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		const FVector TraceStart = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
		const FVector TraceEnd = GetActorLocation() - FVector(0.0f, 0.0f, 200.0f);

		FHitResult Hit;

		// Check if car is flipped on its side, and check if the car is in the air
		const bool bInAir = !GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
		const bool bNotGrounded = FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 0.1f;

		// Only allow in air-movement if we are not on the ground, or are in the air
		if (bInAir || bNotGrounded)
		{
			const float ForwardInput = InputComponent->GetAxisValue(NAME_ThrottleInput);
			const float RightInput = InputComponent->GetAxisValue(NAME_SteeringInput);

			// If car is grounded, allow player to roll the car over
			const float AirMovementForcePitch = 0.1f;
			const float AirMovementForceRoll = (!bInAir && bNotGrounded ? 7.5f : 0.1f);

			if (UPrimitiveComponent* VehicleMesh = Vehicle4W->UpdatedPrimitive)
			{
				const FVector MovementVector = FVector(RightInput * -AirMovementForceRoll, ForwardInput * AirMovementForcePitch, 0.0f) * DeltaTime * 200.0f;
				const FVector NewAngularMovement = GetActorRotation().RotateVector(MovementVector);

				VehicleMesh->SetPhysicsAngularVelocity(NewAngularMovement, true);
			}
		}
	}
	*/
}

void AMyWheeledVehicle::IncrementHealth(float inc)
{
	PlayerHealth += inc;
}

void AMyWheeledVehicle::IncrementTurretCount(int inc)
{
	TurretCount += inc;
}

void AMyWheeledVehicle::IncrementTimeRemaining(float inc)
{
	TimeRemaining += inc;
}

FString AMyWheeledVehicle::GetTimerString()
{
	int min, sec, mil;
	FText M, S, Mi;
	FString Minutes, Seconds, Milli;

	FNumberFormattingOptions Format;
	Format.MinimumIntegralDigits = 2;

	min = (int)(TimeRemaining / 60.0f);
	M = FText::AsNumber(min, &Format);
	Minutes = M.ToString();

	sec = (int)(TimeRemaining - (min * 60.0f));
	S = FText::AsNumber(sec, &Format);
	Seconds = S.ToString();

	mil = (int)(((TimeRemaining * 60.0f) - (sec * 60.0f) - (min * 3600.0f)) * (100.0f / 60.0f));
	Mi = FText::AsNumber(mil, &Format);
	Milli = Mi.ToString();
	
	FString Timer = Minutes + ":" + Seconds + ":" + Milli;
	return Timer;
}

void AMyWheeledVehicle::ApplyForce(float force, FVector start)
{
	MeshActor->AddImpulseAtLocation(MeshActor->GetUpVector() * force, start);
}

uint8 AMyWheeledVehicle::CheckTraceCol(FVector start, FVector end)
{
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_Visibility, CollisionParams))
	{
		return OutHit.bBlockingHit;
	}

	return 0;
}

void AMyWheeledVehicle::HandleWheels()
{
	float dist = 90.0f;
	float strength = 40.0f;

	// Line Traces
	FVector FR = MeshActor->GetSocketLocation(FName("Front_Right"));
	FVector FL = MeshActor->GetSocketLocation(FName("Front_Left"));
	FVector BR = MeshActor->GetSocketLocation(FName("Back_Right"));
	FVector BL = MeshActor->GetSocketLocation(FName("Back_Left"));
	FVector EndFR = ((-GetActorUpVector() * dist) + FR);
	FVector EndFL = ((-GetActorUpVector() * dist) + FL);
	FVector EndBR = ((-GetActorUpVector() * dist) + BR);
	FVector EndBL = ((-GetActorUpVector() * dist) + BL);

	OnGround = (
		CheckTraceCol(FR, EndFR) +
		CheckTraceCol(FL, EndFL) +
		CheckTraceCol(BR, EndBR) +
		CheckTraceCol(BL, EndBL));

	if (CheckTraceCol(FR, EndFR) != 0)
		ApplyForce(strength * (dist - TraceDist(FR, EndFR)), FR);
	if (CheckTraceCol(FL, EndFL) != 0)
		ApplyForce(strength * (dist - TraceDist(FL, EndFL)), FL);
	if (CheckTraceCol(BR, EndBR) != 0)
		ApplyForce(strength * (dist - TraceDist(BR, EndBR)), BR);
	if (CheckTraceCol(BL, EndBL) != 0)
		ApplyForce(strength * (dist - TraceDist(BL, EndBL)), BL);
}

float AMyWheeledVehicle::TraceDist(FVector start, FVector end)
{
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_Visibility, CollisionParams))
	{
		return OutHit.Distance;
	}

	return 0.0f;
}
