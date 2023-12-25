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

UCLASS()
class ALEPH_API AAlephCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAlephCharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components") UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components|Flashlight") USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components|Flashlight") USpotLightComponent* InnerLight;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components|Flashlight") USpotLightComponent* OuterLight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components|Interaction") UPrimitiveComponent* HitComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components|Interaction") UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components") UHealthComponent* HealthComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components") UStaminaComponent* StaminaComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* LookAction;

protected:
	virtual void BeginPlay() override;

};
