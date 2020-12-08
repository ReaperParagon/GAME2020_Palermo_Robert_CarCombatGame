// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARotateActor::ARotateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Pitchvalue = Yawvalue = Rollvalue = 0.0f;

	TargetDirection = FVector(0.0f, 0.0f, 0.0f);

	MeshActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshActor"));
	MeshActor->SetupAttachment(RootComponent);

	BobAngle = 0.0f;
	BobScale = 0.2f;
}

// Called when the game starts or when spawned
void ARotateActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotating the Actor's Mesh
	FQuat QuatRotation = FQuat(FRotator(Pitchvalue, Yawvalue, Rollvalue));
	MeshActor->AddLocalRotation(QuatRotation, false, 0, ETeleportType::None);

	// Bobing up and down with a sin function
	float Range = sin(BobAngle) * BobScale;

	BobAngle += (DeltaTime);
	if (BobAngle > 360.0f)
		BobAngle -= 360.0f;

	// Adding Translation
	AddActorWorldOffset(TargetDirection * Range);
	// AddActorLocalOffset(TargetDirection * Range);
	// AddActorLocalTransform(Transform, false, 0, ETeleportType::None);
}

