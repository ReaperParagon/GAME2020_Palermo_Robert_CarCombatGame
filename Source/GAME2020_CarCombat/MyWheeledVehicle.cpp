// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWheeledVehicle.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"

// static const FName NAME_SteeringInput("Steer");
// static const FName NAME_ThrottleInput("Throttle");

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
	MeshActor->SetupAttachment(RootComponent);

	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
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
}

void AMyWheeledVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IncrementTimeRemaining(-DeltaTime);

	UpdateInAirControl(DeltaTime);
}

void AMyWheeledVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent->BindAxis(NAME_ThrottleInput, this, &AMyWheeledVehicle::ApplyThrottle);
	// PlayerInputComponent->BindAxis(NAME_SteeringInput, this, &AMyWheeledVehicle::ApplySteering);
	// PlayerInputComponent->BindAxis("LookVertical", this, &AMyWheeledVehicle::LookVert);
	// PlayerInputComponent->BindAxis("LookHorizontal", this, &AMyWheeledVehicle::LookHoriz);
	// 
	// PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AMyWheeledVehicle::OnHandBrakePressed);
	// PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AMyWheeledVehicle::OnHandBrakeReleased);
}

void AMyWheeledVehicle::ApplyThrottle(float val)
{
	// GetVehicleMovementComponent()->SetThrottleInput(val);
}

void AMyWheeledVehicle::ApplySteering(float val)
{
	// GetVehicleMovementComponent()->SetSteeringInput(val);
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
