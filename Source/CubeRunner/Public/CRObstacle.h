// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRObstacle.generated.h"

UCLASS()
class CUBERUNNER_API ACRObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	ACRObstacle();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Cube;
};
