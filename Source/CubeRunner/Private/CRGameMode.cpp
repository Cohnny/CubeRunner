// Fill out your copyright notice in the Description page of Project Settings.


#include "CRGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ACRGameMode::ACRGameMode()
{
	CurrentLevelIndex = 0;
	bGameEnded = false;
}



void ACRGameMode::BeginPlay()
{
	Super::BeginPlay();

	Controller = GetWorld()->GetFirstPlayerController();
	
	FInputModeGameOnly InputMode;
	Controller->SetInputMode(InputMode);
	Controller->bShowMouseCursor = false;
	
	CheckLevel();
	UGameplayStatics::PlaySound2D(this, BackgroundMusic);
}

void ACRGameMode::CheckLevel()
{
	if (bGameEnded) return;
	
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	Levels.Find(CurrentLevelName, CurrentLevelIndex);

	if (CurrentLevelIndex < Levels.Num() - 1)
	{
		NextLevel = Levels[CurrentLevelIndex + 1];
	}
	else
	{
		NextLevel = "End";
	}
}

void ACRGameMode::EndGame()
{
	if (bGameEnded) return;
	
	FString LevelString = GetWorld()->GetMapName();
	LevelString.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	FName LevelToLoad = FName(*LevelString);

	UGameplayStatics::OpenLevel(this, LevelToLoad, true);
}

void ACRGameMode::LevelComplete()
{
	if (DefaultLevelCompleteWidget)
	{
		LevelCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultLevelCompleteWidget);
		LevelCompleteWidget->AddToViewport();
		UGameplayStatics::PlaySound2D(this, VictorySound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No default level complete widget selected."));
	}

	GetWorldTimerManager().SetTimer(LevelSwapTimer, this, &ACRGameMode::LoadNextLevel, 2.0f, false);
}

void ACRGameMode::LoadNextLevel()
{
	if (Levels.Contains(NextLevel))
	{
		FName LevelToLoad = FName(*NextLevel);
		UGameplayStatics::OpenLevel(this, LevelToLoad, true);
	}
	else
	{
		if (LevelCompleteWidget)
		{
			LevelCompleteWidget->RemoveFromParent();

			if (DefaultGameCompleteWidget)
			{
				bGameEnded = true;
				GameCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultGameCompleteWidget);
				GameCompleteWidget->AddToViewport();

				Controller->bShowMouseCursor = true;
				FInputModeUIOnly InputMode;
				Controller->SetInputMode(InputMode);
				UGameplayStatics::PlaySound2D(this, VictorySound);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No default game complete widget selected."));
			}
		}
	}
}