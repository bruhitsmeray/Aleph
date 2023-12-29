// Fill out your copyright notice in the Description page of Project Settings.


#include "AlephCharacter.h"
#include "imgui.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/PBPlayerMovement.h"

#include "HealthComponent.h"
#include "StaminaComponent.h"

AAlephCharacter::AAlephCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UPBPlayerMovement>(ACharacter::CharacterMovementComponentName)) {
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	Camera->SetWorldLocation(FVector(0, 0, 70));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));
}

void AAlephCharacter::BeginPlay() {
	Super::BeginPlay();
}

void AAlephCharacter::LookUp(float Axis) {
	AddControllerPitchInput(Axis * Sensitivity * GetWorld()->GetDeltaSeconds());
	UpdateParallaxUI();
}

void AAlephCharacter::Turn(float Axis) {
	AddControllerYawInput(Axis * Sensitivity * GetWorld()->GetDeltaSeconds());
	UpdateParallaxUI();
}

void AAlephCharacter::MoveForward(float Value) {
	if (!FMath::IsNearlyZero(Value)) {
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AAlephCharacter::MoveSide(float Value) {
	if (!FMath::IsNearlyZero(Value)) {
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AAlephCharacter::BeginCrouch() {
	Crouch();
}

void AAlephCharacter::EndCrouch() {
	UnCrouch();
}

void AAlephCharacter::BeginSprint() {
}

void AAlephCharacter::EndSprint() {
}

void AAlephCharacter::Dash() {
	SetDashDistance(2.0f);
	LaunchCharacter(FVector(Camera->GetForwardVector().X * GetDashDistance(), Camera->GetForwardVector().Y * GetDashDistance(), GetActorLocation().Z), false, false);
}

void AAlephCharacter::UpdateParallaxUI() {
	PitchRate = UKismetMathLibrary::FClamp((Camera->GetComponentRotation().Pitch - CameraRot.Pitch) + PitchRate, MinMaxPitchRate * -1.0f, MinMaxPitchRate);
	YawRate = UKismetMathLibrary::FClamp((Camera->GetComponentRotation().Yaw - CameraRot.Yaw) + YawRate, MinMaxYawRate * -1.0f, MinMaxYawRate);
	CameraRot = Camera->GetComponentRotation();
	PitchParallaxOffset = UKismetMathLibrary::FInterpTo(PitchParallaxOffset, PitchRate, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
	YawParallaxOffset = UKismetMathLibrary::FInterpTo(YawParallaxOffset, YawRate, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);

	if (ParallaxCollection != nullptr) {
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), ParallaxCollection, "Offset", FLinearColor(YawParallaxOffset, PitchParallaxOffset, 0.0f));
	}

	PitchRate = UKismetMathLibrary::FInterpTo(PitchRate, 0.0f, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
	YawRate = UKismetMathLibrary::FInterpTo(YawRate, 0.0f, GetWorld()->GetDeltaSeconds(), ParallaxSpeed);
}

void AAlephCharacter::DisplayImGuiDebugWindow() {
	const FString GameName = FApp::GetProjectName();

	ImGui::Begin(TCHAR_TO_ANSI(*GameName), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("Velocity: %.1f", GetVelocity().Size());
	ImGui::End();
}

void AAlephCharacter::SetDashDistance(float Distance) {
	DashDistance = Distance * 1000.0f;
}

void AAlephCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	DisplayImGuiDebugWindow();
}

void AAlephCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &AAlephCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AAlephCharacter::Turn);
	PlayerInputComponent->BindAxis("Forward", this, &AAlephCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Side", this, &AAlephCharacter::MoveSide);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAlephCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAlephCharacter::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AAlephCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AAlephCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AAlephCharacter::Dash);
}