// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CRGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CUBERUNNER_API ACRGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACRGameMode();
	
	void EndGame();
	void LevelComplete();
	void LoadNextLevel();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UUserWidget> DefaultLevelCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	UUserWidget* LevelCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UUserWidget> DefaultGameCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	UUserWidget* GameCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* BackgroundMusic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* VictorySound;

	FTimerHandle LevelSwapTimer;
	
private:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels", meta = (AllowPrivateAccess = "true"))
	TArray<FString> Levels;

	APlayerController* Controller;

	int32 CurrentLevelIndex;
	FString NextLevel;

	bool bGameEnded;

	void CheckLevel();
};
