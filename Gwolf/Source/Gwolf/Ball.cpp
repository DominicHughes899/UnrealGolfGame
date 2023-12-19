// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	RootComponent = BallMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallAsset(TEXT("StaticMesh'/Game/Core/Meshes/SM_Ball.SM_Ball'"));
	if (BallAsset.Succeeded()) { BallMesh->SetStaticMesh(BallAsset.Object); }

	
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowAsset(TEXT("StaticMesh'/Game/Core/Meshes/SM_Arrow.SM_Arrow'"));
	if (ArrowAsset.Succeeded()) { ArrowMesh->SetStaticMesh(ArrowAsset.Object); }

	ArrowMesh->SetupAttachment(RootComponent);

	BallMesh->SetSimulatePhysics(true);
	
	ArrowMesh->SetVisibility(false);
	
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StopIfSlow();

	UpdateArrow();

	CheckOverlaps();


}

void ABall::StopIfSlow()
{
	FVector CurrentVel3D = GetVelocity();
	FVector2d CurrentVel = FVector2d(CurrentVel3D.X, CurrentVel3D.Y);

	if (CurrentVel.Length() > 15.f)
	{
		IsMoving = true;
	}

	if (IsMoving)
	{
		if (CurrentVel.Length() < 15.f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, "Stopping because slow");

			BallMesh->SetPhysicsLinearVelocity(FVector(0.f, 0.f, CurrentVel3D.Z));
			BallMesh->SetPhysicsAngularVelocityInRadians(FVector(0.f, 0.f, 0.f));
			IsMoving = false;
		}
	}
}

void ABall::BeginCharging()
{
	ArrowMesh->SetVisibility(true);
	IsChargingShot = true;
}

void ABall::EndCharging(FVector impulse)
{
	ArrowMesh->SetVisibility(false);
	IsChargingShot = false;
	ArrowMesh->SetWorldScale3D(FVector(0.4f, 1.f, 1.f));

	BallMesh->AddImpulse(impulse);
}

void ABall::UpdateArrow()
{
	if (IsChargingShot)
	{
		// Orrient arrow to player
		ArrowMesh->SetWorldRotation(ArrowRotation);
		// Scale arrow
		ArrowMesh->SetWorldScale3D(FVector(ArrowScale, 1.f, 1.f));
	}
}

void ABall::CheckOverlaps()
{
	TArray<UPrimitiveComponent*> Components;
	GetOverlappingComponents(Components);
	
	IsInHole = false;
	IsInBounds = false;

	if (Components.Num() != 0)
	{
		for (int i = 0; i < Components.Num(); i++)
		{
			if (Components[i]->ComponentHasTag(FName("Hole")))
			{
				IsInHole = true;
				break;
			}
		}

		for (int i = 0; i < Components.Num(); i++)
		{
			if (Components[i]->ComponentHasTag(FName("Bounds")))
			{
				IsInBounds = true;
				break;
			}
		}
	}
}

