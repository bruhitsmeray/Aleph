// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlephCharacter.h"
#include "Engine/LocalPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SpotLightComponent.h"

#include "GameFramework/Controller.h"
#include "imgui.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"


//////////////////////////////////////////////////////////////////////////
// AAlephCharacter

AAlephCharacter::AAlephCharacter() {
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
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
	Flashlight->SetRelativeLocation(FVector(5.0f, 20.0f, -12.0f));
	Flashlight->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

void AAlephCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime); 
	
	const FString Name = GetActorNameOrLabel();
	const FString GameName = FApp::GetProjectName();
	ImGui::Begin(TCHAR_TO_ANSI(*Name), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderInt("Current Health", &Health, 1, MaxHealth);
	ImGui::End();

	if (Health != OldHealth) {
		OldHealth = Health;
		HealthComponent->SetHealth(Health);
	}

	ImGui::Begin(TCHAR_TO_ANSI(*GameName), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("Velocity: %.1f", GetVelocity().Size());
	ImGui::End();
}

void AAlephCharacter::BeginJump() {
	GetCharacterMovement()->JumpZVelocity = GetVelocity().Size() * BaseWalkMultiplier;
	Jump();
}

void AAlephCharacter::BeginSprint() {
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * BaseWalkMultiplier;
}

void AAlephCharacter::EndSprint() {
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAlephCharacter::BeginPlay() {
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	Health = HealthComponent->GetPureHealth();
	MaxHealth = HealthComponent->GetPureMaxHealth();
	OldHealth = Health;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAlephCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AAlephCharacter::BeginJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AAlephCharacter::BeginSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AAlephCharacter::EndSprint);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAlephCharacter::Move);

		// Looking
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