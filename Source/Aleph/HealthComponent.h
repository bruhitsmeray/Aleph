// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALEPH_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure) float GetHealth() { return Health / 100.0f; }
	UFUNCTION(BlueprintPure) float GetMaxHealth() { return MaxHealth / 100.0f; }
	UFUNCTION(BlueprintPure) int GetPureHealth() { return Health; }
	UFUNCTION(BlueprintPure) int GetPureMaxHealth() { return MaxHealth; }
	UFUNCTION(BlueprintCallable) void SetHealth(float NewHealth) { Health = NewHealth; }
	UFUNCTION(BlueprintCallable) void SetMaxHealth(float NewMaxHealth) { MaxHealth = NewMaxHealth; }

protected:
	virtual void BeginPlay() override;

private:	
	UFUNCTION()	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	int MaxHealth;
	int Health;
};
