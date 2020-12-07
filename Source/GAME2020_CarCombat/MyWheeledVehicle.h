// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/AudioComponent.h"
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

	/** Throttle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float ThrottlePower;

	/** Turning Power */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float TurnPower;

	/** Bullet Timer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float BulletTimer;

	/** Bullet Timer Max */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float TimerMax;

	/** Bullet Spawn Point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	FVector BulletSpawn;

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

	/** Actor's Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class USoundCue* SoundEngine;

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

	/** Get if the Player is defeated */
	UFUNCTION(BlueprintCallable, Category = "Player")
	bool GetPlayerDefeated();

	/** Get if the Player is victorious */
	UFUNCTION(BlueprintCallable, Category = "Player")
	bool GetPlayerVictory();

	/** Apply force */
	void ApplyForce(float force, FVector start);

	/** Check Collision for Line Trace */
	uint8 CheckTraceCol(FVector start, FVector end);

	/** Handle the wheels */
	void HandleWheels();

	/** Distance to Ground */
	float TraceDist(FVector start, FVector end);

	UAudioComponent* EngineAudioComponent;

	uint8 OnGround;
	float ThrottleVal;
	float SteerVal;
};
