// Fill out your copyright notice in the Description page of Project Settings.

#include "StartMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UStartMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr && JoinButton != nullptr && CancelButton != nullptr && JoinGameButton != nullptr)) return false;
	
	HostButton->OnClicked.AddDynamic(this, &UStartMenu::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UStartMenu::OpenJoinMenu);
	CancelButton->OnClicked.AddDynamic(this, &UStartMenu::OpenStartMenu);
	JoinGameButton->OnClicked.AddDynamic(this, &UStartMenu::JoinServer);
	QuitButton->OnClicked.AddDynamic(this, &UStartMenu::QuitGame);
	
	
	return true;
}

void UStartMenu::HostServer()
{
	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->Host();
}

void UStartMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UStartMenu::QuitGame()
{
	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->QuitGame();
}

void UStartMenu::OpenStartMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UStartMenu::JoinServer()
{
	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->Join(IPAddressField->GetText().ToString());
}

void UStartMenu::SetMenuInterface(IMenuInterface* MenuIF)
{
	MenuInterface = MenuIF;
}

void UStartMenu::Setup()
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

void UStartMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	this->RemoveFromViewport();

	APlayerController* PlayerController = InWorld->GetFirstPlayerController();
	FInputModeGameOnly InputData;
	InputData.SetConsumeCaptureMouseDown(true);

	PlayerController->SetInputMode(InputData);
	PlayerController->bShowMouseCursor = false;
}