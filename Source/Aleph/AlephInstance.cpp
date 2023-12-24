// Fill out your copyright notice in the Description page of Project Settings.


#include "AlephInstance.h"
#include "Misc/MessageDialog.h"

void UAlephInstance::Init() {
	Super::Init();

	#if !UE_EDITOR
		const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
		const FString LockFilePath = FPlatformProcess::UserTempDir() + ProjectSettings.ProjectID.ToString();
		if (!IFileManager::Get().CreateFileWriter(*LockFilePath, 0) && true)
		{
			FPlatformApplicationMisc::RequestMinimize();
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Only one instance of the game can be running at one time.")), &GameName);
			FPlatformMisc::RequestExit(0);
		}
	#endif
}

void UAlephInstance::StartGameInstance() {
	Super::StartGameInstance();
}

void UAlephInstance::Shutdown() {
	Super::Shutdown();
}
