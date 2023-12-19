// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/StaticMeshComponent.h"
#include "Ball.h"

#include "Gwolf_Character.generated.h"

UCLASS()
class GWOLF_API AGwolf_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGwolf_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUpdateScore(int newScore);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Player location indicator
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LocationIndicatorMesh;

	void AnimateLocationIndicator(float dT);
	float IndictorAnimationTimer;

	// ======= Movemet =========
	void CameraPitch(float Value);
	void MoveForward(float Value) { AddMovementInput(GetActorForwardVector(), Value); }
	void MoveRight(float Value) { AddMovementInput(GetActorRightVector(), Value); }

	bool CanPitch = true;

	UFUNCTION(BlueprintCallable)
	void SetCanPitch(bool canPitch) { CanPitch = canPitch; }
	

	//======Ball=======
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABall> BallBlueprint;

	//UFUNCTION(Server, Reliable, Blueprintcallable)
	void SpawnBall();

	ABall* Ball;

	// ========== Ball Interaction ===========
	void SnapToBall(); 
	void LockOnBall();

	void BeginCharge();
	void FinishCharge();

	void ChargeShot(float Value);
	void UpdateBall();

	bool IsLockedOn = false;
	bool IsCharging = false;

	float Power = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxPower = 10000.f;

	// ========= Gameplay ===========
	void CheckWaiting();
	void UpdateScore();
	void CheckBallInHole();


	bool WaitForShot = false;
	int Score = 0;
	bool IsBallInHole = false;



};
