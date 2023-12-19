// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "CourseHole.generated.h"

UCLASS()
class GWOLF_API ACourseHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACourseHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// ======= Root component =======
	UPROPERTY(EditAnywhere, Category = "Root")
	USceneComponent* DummyRoot;

	// ======= Boundary ========
	UPROPERTY(EditAnywhere, Category = "Boundary")
	UBoxComponent* BoundaryBox;
	
	// ====== Flag =======
	UPROPERTY(EditAnywhere, Category = "Flag Hole")
	UBoxComponent* FlagBox;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FlagMesh;

};
