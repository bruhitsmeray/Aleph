// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"

class UCharacterMovementComponent;

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	MaxStamina = 100;
}


void UStaminaComponent::DrainStamina() {
	Stamina = FMath::Clamp(Stamina - DrainStaminaRate, 0, MaxStamina);
}

void UStaminaComponent::RegenStamina() {
	if (Stamina < MaxStamina) {
		Stamina = FMath::Clamp(Stamina + RegenStaminaRate, 0, MaxStamina);
	}
	else {
		bCanRegenStamina = false;
	}
}

void UStaminaComponent::BeginPlay() {
	Super::BeginPlay();

	Stamina = MaxStamina;
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}