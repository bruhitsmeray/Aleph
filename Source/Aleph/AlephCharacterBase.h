// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlephCharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USpotLightComponent;
class UPhysicsHandleComponent;
class UHealthComponent;
class UStaminaComponent;

class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS(config = Game)
class ALEPH_API AAlephCharacterBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true")) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Flashlight", meta = (AllowPrivateAccess = "true")) USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Flashlight", meta = (AllowPrivateAccess = "true")) USpotLightComponent* InnerLight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Flashlight", meta = (AllowPrivateAccess = "true")) USpotLightComponent* OuterLight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Interaction", meta = (AllowPrivateAccess = "true")) UPrimitiveComponent* HitComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Interaction", meta = (AllowPrivateAccess = "true")) UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true")) UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true")) UStaminaComponent* StaminaComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* LookAction;

public:
	AAlephCharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	// DO NOT USE THIS IN RELEASE
	int Health;
	int MaxHealth;
	int OldHealth;

};
