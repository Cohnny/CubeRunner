// Fill out your copyright notice in the Description page of Project Settings.


#include "CRPawn.h"

#include "CREndPoint.h"
#include "CRGameMode.h"
#include "CRObstacle.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACRPawn::ACRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = Cube;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Cube);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	ForwardForce = 2000.f;
	SideForce = 5.f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACRPawn::BeginPlay()
{
	Super::BeginPlay();

	Cube->OnComponentHit.AddDynamic(this, &ACRPawn::OnHit);
	Cube->OnComponentBeginOverlap.AddDynamic(this, &ACRPawn::OnBeginOverlap);
	GameMode = Cast<ACRGameMode>(GetWorld()->GetAuthGameMode());

	bGameEnded = false;
	Cube->SetSimulatePhysics(true);
	Mass = Cube->GetMass();

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PawnMappingContext, 0);
		}
	}

	if (DefaultHUD)
	{
		HUD = CreateWidget<UUserWidget>(GetWorld(), DefaultHUD);
		HUD->AddToViewport();
	}
}

// Called every frame
void ACRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGameEnded)
	{
		FVector Impulse = FVector(Mass * ForwardForce * DeltaTime, 0.0f, 0.0f);
		Cube->AddImpulse(Impulse);

		FVector ActorLocation = GetActorLocation();
		if (ActorLocation.Z < -30)
		{
			GameEnded();
		}
	}

	DeltaSeconds = DeltaTime;
}

// Called to bind functionality to input
void ACRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACRPawn::Move);
	}
}

void ACRPawn::OnHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	ACRObstacle* Obstacle = Cast<ACRObstacle>(Other);

	if (Obstacle)
	{
		UGameplayStatics::PlaySound2D(this, CrashSound);
		GameEnded();
	}
}

void ACRPawn::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* Othercomp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACREndPoint* LevelEnd = Cast<ACREndPoint>(OtherActor);

	if (LevelEnd)
	{
		GameMode->LevelComplete();
	}
}

void ACRPawn::GameEnded()
{
	if (bGameEnded) return;

	bGameEnded = true;

	GetWorldTimerManager().SetTimer(EndGameTimer, this, &ACRPawn::EndGame, 2.0f, false);
}

void ACRPawn::EndGame()
{
	GameMode->EndGame();
}

void ACRPawn::Move(const FInputActionValue& Value)
{
	if (!bGameEnded)
	{
		const FVector2d MovementVector = Value.Get<FVector2d>();

		const FVector RightForce = GetActorRightVector() * MovementVector.X * SideForce * Mass;
		Cube->AddImpulse(RightForce);
	}
}