// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"

#include "Components/Button.h"
#include "PuzzlePlatformsGameInstance.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(ContinueButton != nullptr && ExitButton != nullptr)) return false;

	ContinueButton->OnClicked.AddDynamic(this, &UInGameMenu::ContinueGame);
	ExitButton->OnClicked.AddDynamic(this, &UInGameMenu::ExitGame);

	return true;
}


void UInGameMenu::LoadInGameMenu()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputData;
	InputData.SetWidgetToFocus(this->TakeWidget());
	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputData);
	PlayerController->bShowMouseCursor = true;
}

void UInGameMenu::ContinueGame()
{
	TearAway();
}

void UInGameMenu::ExitGame()
{
	TearAway();
	UPuzzlePlatformsGameInstance* PuzzlePlatformsGameInstance = Cast<UPuzzlePlatformsGameInstance>(GetWorld()->GetGameInstance());
	PuzzlePlatformsGameInstance->LoadMainMenu();
}

void UInGameMenu::TearAway()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputData;
	InputData.SetConsumeCaptureMouseDown(true);

	PlayerController->SetInputMode(InputData);
	PlayerController->bShowMouseCursor = false;

	this->RemoveFromViewport();
}