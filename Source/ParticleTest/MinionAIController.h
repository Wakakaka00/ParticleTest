// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Minion.h"
#include "MinionAIController.generated.h"

UCLASS()
class PARTICLETEST_API AMinionAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	
	FVector currentVelocity;
	FVector acceleration;
	float accelerationForce = 0.5f;
	float maxMagnitude = 2.0f;
	float distanceToPlayer;
	void MoveToPlayer();

public:
	AMinionAIController();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	APawn* minionPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	ACharacter* playerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	class AMinion* minionActor;
};
