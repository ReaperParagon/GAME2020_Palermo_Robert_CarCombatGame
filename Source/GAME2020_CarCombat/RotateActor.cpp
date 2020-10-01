// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateActor.h"

// Sets default values
ARotateActor::ARotateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Pitchvalue = Yawvalue = Rollvalue = 0.0f;
	X = MinX = MaxX = Y = MinY = MaxY = Z = MinZ = MaxZ = 0.0f;

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

	FQuat QuatRotation = FQuat(FRotator(Pitchvalue, Yawvalue, Rollvalue));
	
	// AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

	if (X != 0.0f || Y != 0.0f || Z != 0.0f)
	{
		if (GetActorTransform().GetLocation().X > MaxX || GetActorTransform().GetLocation().X < MinX)
			X *= -1.0f;
		if (GetActorTransform().GetLocation().Y > MaxY || GetActorTransform().GetLocation().Y < MinY)
			Y *= -1.0f;
		if (GetActorTransform().GetLocation().Z > MaxZ || GetActorTransform().GetLocation().Z < MinZ)
			Z *= -1.0f;
	}

	FTransform Transform = FTransform(QuatRotation, FVector(X, Y, Z), GetActorScale());

	AddActorLocalTransform(Transform, false, 0, ETeleportType::None);
}

