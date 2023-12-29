// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PBPlayerCharacter.h"
#include "AlephCharacter.generated.h"

class UCameraComponent;
class UHealthComponent;
class UStaminaComponent;

UCLASS()
class ALEPH_API AAlephCharacter : public APBPlayerCharacter
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true")) UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true")) UHealthComponent* HealthComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true")) UStaminaComponent* StaminaComponent;

public:
	AAlephCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure) float GetDashDistance() { return DashDistance; }
	UFUNCTION(BlueprintCallable) void SetDashDistance(float Distance);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parallax UI") UMaterialParameterCollection* ParallaxCollection;

protected:
	virtual void BeginPlay() override;

	void LookUp(float Axis);
	void Turn(float Axis);

	void MoveForward(float Value);
	void MoveSide(float Value);

	void BeginCrouch();
	void EndCrouch();
	void BeginSprint();
	void EndSprint();

	void Dash();

	void UpdateParallaxUI();
	void DisplayImGuiDebugWindow();

private:
	float Sensitivity = 20.0f;

	float YawRate;
	float PitchRate;
	float YawParallaxOffset;
	float PitchParallaxOffset;
	float ParallaxSpeed = 12.0f;
	FRotator CameraRot;
	float MinMaxYawRate = 2.0f;
	float MinMaxPitchRate = 12.0f;

	float DashDistance;

};
