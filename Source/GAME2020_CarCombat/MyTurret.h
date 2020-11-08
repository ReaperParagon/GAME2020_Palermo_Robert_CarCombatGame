// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTurret.generated.h"

UCLASS()
class GAME2020_CARCOMBAT_API AMyTurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTurret();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	/** Base of the Turret */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshBase;

	/** Head of the Turret */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshHead;

	/** Rotation Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;

	/** Direction towards the Player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	FVector TargetDirection;

	/** Time in Seconds for Turret to Shoot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	float ShootTime;

	float BobAngle;
	float BobScale;
};
