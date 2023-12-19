// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "Ball.generated.h"

UCLASS()
class GWOLF_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ======== Mesh Components =========
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArrowMesh;

	// ======== Movement ===========

	void StopIfSlow();
	bool GetIsMoving() { return IsMoving; }

	bool IsMoving = false;

	// ======== Player Interaction ==========

	void BeginCharging();
	void EndCharging(FVector impulse);

	void UpdateArrow();

	void SetArrowYaw(float newYaw) { ArrowRotation.Yaw = newYaw; }
	void SetArrowScale(float Value) { ArrowScale = FMath::Lerp(0.4f, 3.f, Value); }

	bool GetIsInHole() { return IsInHole; }

	bool IsChargingShot = false;
	FRotator ArrowRotation = FRotator(0.f,0.f,0.f);
	float ArrowScale;

	// ======== Hole Interaction =============
	void CheckOverlaps();

	bool IsInBounds = false;

	
	bool IsInHole = false;
	



};
