// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotateActor.generated.h"

UCLASS()
class GAME2020_CARCOMBAT_API ARotateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotateActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Movement");
	float Pitchvalue;

	UPROPERTY(EditAnywhere, Category = "Movement");
	float Yawvalue;

	UPROPERTY(EditAnywhere, Category = "Movement");
	float Rollvalue;

private:

	/** Actor's Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshActor;

	/** Direction of Movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateActor", meta = (AllowPrivateAccess = "true"))
	FVector TargetDirection;

	float BobAngle;
	float BobScale;
};
