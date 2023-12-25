// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALEPH_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bCanRegenStamina;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int DrainStaminaRate = 2;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int RegenStaminaRate = 2;

	UFUNCTION(BlueprintPure) float GetStamina() { return Stamina / 100.0f; }
	UFUNCTION(BlueprintPure) float GetMaxStamina() { return MaxStamina / 100.0f; }
	UFUNCTION(BlueprintPure) int GetPureStamina() { return Stamina; }
	UFUNCTION(BlueprintPure) int GetPureMaxStamina() { return MaxStamina; }

	UFUNCTION(BlueprintCallable) void SetStamina(float NewStamina) { Stamina = NewStamina; }
	UFUNCTION(BlueprintCallable) void SetMaxStamina(float NewMaxStamina) { MaxStamina = NewMaxStamina; }

	UFUNCTION(BlueprintCallable) void DrainStamina();
	UFUNCTION(BlueprintCallable) void RegenStamina();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int MaxStamina;
	int Stamina;
		
};
