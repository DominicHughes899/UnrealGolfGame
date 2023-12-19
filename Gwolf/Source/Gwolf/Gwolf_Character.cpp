// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwolf_Character.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGwolf_Character::AGwolf_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialise Location Indicator Mesh
	LocationIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LocationIndicator"));
	LocationIndicatorMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> IndicatorAsset(TEXT("StaticMesh'/Game/Core/Meshes/SM_PlayerMarker.SM_PlayerMarker'"));
	if (IndicatorAsset.Succeeded())
	{
		LocationIndicatorMesh->SetStaticMesh(IndicatorAsset.Object);
	}

}

// Called when the game starts or when spawned
void AGwolf_Character::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnBall();
}

void AGwolf_Character::UpdateScore()
{
	Score++;
	OnUpdateScore(Score);
}

void AGwolf_Character::CheckBallInHole()
{
	if (!IsBallInHole)
	{
		IsBallInHole = Ball->GetIsInHole();
	}
}

// Called every frame
void AGwolf_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimateLocationIndicator(DeltaTime);

	UpdateBall();

	CheckWaiting();
	CheckBallInHole();
	if (IsBallInHole)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, "BallInHole");
	}
	
}

// Called to bind functionality to input
void AGwolf_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("IAx_Camera_Pitch", this, &AGwolf_Character::CameraPitch);
	PlayerInputComponent->BindAxis("IAx_Camera_Yaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("IAx_MoveForward", this, &AGwolf_Character::MoveForward);
	PlayerInputComponent->BindAxis("IAx_MoveRight", this, &AGwolf_Character::MoveRight);
	PlayerInputComponent->BindAxis("IAx_Shot_Charge", this, &AGwolf_Character::ChargeShot);

	PlayerInputComponent->BindAction("IA_SnapToBall", IE_Pressed, this, &AGwolf_Character::SnapToBall);
	PlayerInputComponent->BindAction("IA_Ready", IE_Pressed, this, &AGwolf_Character::LockOnBall);
	PlayerInputComponent->BindAction("IA_HitBall", IE_Pressed, this, &AGwolf_Character::BeginCharge);
	PlayerInputComponent->BindAction("IA_HitBall", IE_Released, this, &AGwolf_Character::FinishCharge);
}

void AGwolf_Character::AnimateLocationIndicator(float dT)
{
	IndictorAnimationTimer += dT;
	float ZOffset = (FMath::Sin(3.f * IndictorAnimationTimer) / 4.f);

	LocationIndicatorMesh->AddLocalOffset(FVector(0.f, 0.f, ZOffset));
}

void AGwolf_Character::CameraPitch(float Value)
{
	if (CanPitch)
	{
		AddControllerPitchInput(Value);
	}
}

void AGwolf_Character::SpawnBall()
{
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.X += 50;

	Ball = GetWorld()->SpawnActor<ABall>(BallBlueprint, SpawnLocation, FRotator(0.f, 0.f, 0.f));
	Ball->SetOwner(this);
}

void AGwolf_Character::SnapToBall()
{
	FVector BallLoc = Ball->GetActorLocation();


	SetActorLocation(FVector(BallLoc.X, BallLoc.Y, GetActorLocation().Z));
}

void AGwolf_Character::LockOnBall()
{
	if (Ball->GetVelocity().Length() == 0 && !IsLockedOn && !WaitForShot)
	{
		IsLockedOn = true;

		SnapToBall();
		GetCharacterMovement()->DisableMovement();

		Ball->BeginCharging();
	}
}

void AGwolf_Character::BeginCharge()
{
	if (IsLockedOn)
	{
		IsCharging = true;
		CanPitch = false;
	}
}

void AGwolf_Character::FinishCharge()
{
	if (IsLockedOn)
	{
		IsCharging = false;
		CanPitch = true;

		Ball->EndCharging(GetActorForwardVector() * Power);
		
		if (Power != 0.f)
		{
			UpdateScore();
		}

		Power = 0.f;

		IsLockedOn = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void AGwolf_Character::ChargeShot(float Value)
{
	if (IsCharging)
	{
		float newPower = Power + Value * 100;
		Power = FMath::Clamp(newPower, 0.f, MaxPower);
	}
}

void AGwolf_Character::UpdateBall()
{
	if (IsLockedOn)
	{
		Ball->SetArrowYaw(GetActorRotation().Yaw);
		Ball->SetArrowScale(Power/MaxPower);
	}
}

void AGwolf_Character::CheckWaiting()
{
	WaitForShot = true;

	if (Ball->GetVelocity().Length() == 0)
	{
		WaitForShot = false;
	}
}



