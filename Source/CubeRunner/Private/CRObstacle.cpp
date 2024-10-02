// Fill out your copyright notice in the Description page of Project Settings.


#include "CRObstacle.h"

// Sets default values
ACRObstacle::ACRObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = Cube;
}

// Called when the game starts or when spawned
void ACRObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}