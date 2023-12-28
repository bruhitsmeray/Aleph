// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlephCharacter.h"
#include "Engine/LocalPlayer.h"
#include "UObject/ConstructorHelpers.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SpotLightComponent.h"

#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AAlephCharacter::AAlephCharacter() {
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 37.5f;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 75.0f;

	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	BaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	BaseCrouchSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0, 0, 80));
	CameraBoom->TargetArmLength = 256.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeLocation(FVector(0, 60, 0));
	FollowCamera->bUsePawnControlRotation = false;

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FindComponentByClass<USkeletalMeshComponent>(), "clavicle_ls");
	Flashlight->SetRelativeLocation(FVector(-0.925f, 20.85f, -12.0f));
	Flashlight->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

void AAlephCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdateParallaxUI();
}

void AAlephCharacter::BeginJump() {
	GetCharacterMovement()->JumpZVelocity = GetVelocity().Size() * BaseWalkMultiplier;
	Jump();
}

void AAlephCharacter::BeginCrouch() {
	Crouch();
}

void AAlephCharacter::EndCrouch() {
	UnCrouch();
}

void AAlephCharacter::BeginSprint() {
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * BaseWalkMultiplier;
}

void AAlephCharacter::EndSprint() {
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAlephCharacter::UpdateParallaxUI() {
	PitchRate = UKismetMathLibrary::FClamp((FollowCamera->GetComponentRotation().Pitch - CameraRot.Pitch) + PitchRate, MinMaxPitchRate * -1.0f, MinMaxPitchRate);
	YawRate = UKismetMathLibrary::FClamp((FollowCamera->GetComponentRotation().Yaw - CameraRot.Yaw) + YawRate, MinMaxYawRate * -1.0f, MinMaxYawRate);
	CameraRot = FollowCamera->GetComponentRotation();
	PitchParallaxOffset = UKismetMathLibrary::FInterpTo(PitchParallaxOffset, PitchRate, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
	YawParallaxOffset = UKismetMathLibrary::FInterpTo(YawParallaxOffset, YawRate, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
		
	if (ParallaxCollection != nullptr) {
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), ParallaxCollection, "Offset", FLinearColor(YawParallaxOffset, PitchParallaxOffset, 0.0f));
	}

	PitchRate = UKismetMathLibrary::FInterpTo(PitchRate, 0.0f, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
	YawRate = UKismetMathLibrary::FInterpTo(YawRate, 0.0f, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
}

void AAlephCharacter::BeginPlay() {
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAlephCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AAlephCharacter::BeginJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AAlephCharacter::BeginCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AAlephCharacter::EndCrouch);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AAlephCharacter::BeginSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AAlephCharacter::EndSprint);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAlephCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAlephCharacter::Look);
	} else {
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAlephCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAlephCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}