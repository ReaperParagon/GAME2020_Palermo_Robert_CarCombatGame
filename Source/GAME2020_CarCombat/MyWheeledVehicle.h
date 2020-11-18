// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyWheeledVehicle.generated.h"

/**
 * 
 */
UCLASS()
class GAME2020_CARCOMBAT_API AMyWheeledVehicle : public APawn
{
	GENERATED_BODY()
public:

	AMyWheeledVehicle();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Throttle and Steering */
	void ApplyThrottle(float val);
	void ApplySteering(float val);

	/** Look around */
	void LookVert(float val);
	void LookHoriz(float val);

	/** HandBrake */
	void OnHandBrakePressed();
	void OnHandBrakeReleased();

	/** Update in air Physics */
	void UpdateInAirControl(float DeltaTime);

	/** Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float PlayerHealth;

	/** Max Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float PlayerHealthMax;

	/** Turrets Remaining */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int TurretCount;

	/** Max Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float TimeRemaining;

protected:

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	/** Actor's Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshActor;

	/** Increment Health Value */
	UFUNCTION(BlueprintCallable, Category = "Player")
	void IncrementHealth(float inc);

	/** Increment Turret Count */
	UFUNCTION(BlueprintCallable, Category = "Player")
	void IncrementTurretCount(int inc);

	/** Increment Time Remaining */
	UFUNCTION(BlueprintCallable, Category = "Player")
	void IncrementTimeRemaining(float inc);

	/** Get Timer as a String in Proper format */
	UFUNCTION(BlueprintCallable, Category = "Player")
	FString GetTimerString();
};
