// Fill out your copyright notice in the Description page of Project Settings.


#include "CourseHole.h"
#include "Components/ActorComponent.h"

// Sets default values
ACourseHole::ACourseHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialise Root
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = DummyRoot;

	// Initialise Boundary
	BoundaryBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Boundary Box"));
	BoundaryBox->SetupAttachment(RootComponent);
	BoundaryBox->ComponentTags.Add(FName("Bounds"));

	BoundaryBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoundaryBox->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

	// Initialise Flag
	FlagBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Flag Box"));
	FlagBox->SetupAttachment(RootComponent);
	FlagBox->ComponentTags.Add(FName("Hole"));
	
	FlagBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	FlagBox->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flag Mesh"));
	FlagMesh->SetupAttachment(FlagBox);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FlagAsset(TEXT("StaticMesh'/Game/Core/Meshes/SM_Flag.SM_Flag'"));
	if (FlagAsset.Succeeded()) { FlagMesh->SetStaticMesh(FlagAsset.Object); }
	
	



}

// Called when the game starts or when spawned
void ACourseHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACourseHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

