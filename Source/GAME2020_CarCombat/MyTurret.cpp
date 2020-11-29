// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTurret.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AMyTurret::AMyTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	MeshBase->SetupAttachment(RootComponent);

	MeshHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	MeshHead->SetupAttachment(MeshBase);

	BobAngle = 0.0f;
	BobScale = 0.2f;
	RotationSpeed = 1.0f;
	TargetDirection = FVector(0.0f, 0.0f, 0.0f);
	ShootTime = 5.0f;
	TurretHealth = 50.0f;
}

// Called when the game starts or when spawned
void AMyTurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FQuat QuatRotation = FQuat(FRotator(0.0f, RotationSpeed, 0.0f));
	
	// Bobing up and down with a sin function
	float Y = sin(BobAngle) * BobScale;

	BobAngle += (DeltaTime);
	if (BobAngle > 360.0f)
		BobAngle -= 360.0f;

	FTransform Transform = FTransform(QuatRotation, FVector(0.0f, 0.0f, Y), GetActorScale());

	MeshHead->AddLocalTransform(Transform, false, 0, ETeleportType::None);

	float acc = (UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation() - MeshHead->GetComponentLocation()).Size() / 30.0f;
	FVector random = FVector(FMath::RandRange(-acc, acc), FMath::RandRange(-acc, acc), FMath::RandRange(-acc, acc));
	TargetDirection = (UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation() - MeshHead->GetComponentLocation()) + random;
}

void AMyTurret::IncrementHealth(float inc)
{
	TurretHealth += inc;
}
