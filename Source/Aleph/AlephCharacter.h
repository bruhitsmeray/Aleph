// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "HealthComponent.h"
#include "AlephCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USpotLightComponent;

class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS(config=Game)
class AAlephCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) USpotLightComponent* Flashlight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parallax UI", meta = (AllowPrivateAccess = "true")) UMaterialParameterCollection* ParallaxCollection;

public:
	AAlephCharacter();
	virtual void Tick(float DeltaTime) override;

	void BeginJump();
	void BeginCrouch();
	void EndCrouch();
	void BeginSprint();
	void EndSprint();

	void UpdateParallaxUI();

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
			

protected:
	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float BaseWalkSpeed;
	float BaseCrouchSpeed;
	float BaseWalkMultiplier = 2.5f;

	float YawRate;
	float PitchRate;
	float YawParallaxOffset;
	float PitchParallaxOffset;
	float ParallaxSpeed = 12.0f;
	FRotator CameraRot;
	float MinMaxYawRate = 2.0f;
	float MinMaxPitchRate = 12.0f;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

