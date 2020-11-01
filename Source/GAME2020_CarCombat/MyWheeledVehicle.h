// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "MyWheeledVehicle.generated.h"

/**
 * 
 */
UCLASS()
class GAME2020_CARCOMBAT_API AMyWheeledVehicle : public AWheeledVehicle
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

protected:

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	/** Set Health Value */
	UFUNCTION(BlueprintCallable, Category = "Player")
	void IncrementHealth(float inc);
};
