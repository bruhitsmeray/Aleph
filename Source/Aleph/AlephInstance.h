// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AlephInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALEPH_API UAlephInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void StartGameInstance() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintPure, Category = "Game") static FString GetGameVersion() {
		FString GameVersion;
		GConfig->GetString(
			TEXT("/Script/EngineSettings.GeneralProjectSettings"),
			TEXT("ProjectVersion"),
			GameVersion,
			GGameIni
		);
		return GameVersion;
	};
};
