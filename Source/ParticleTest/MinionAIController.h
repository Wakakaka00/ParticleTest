// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Minion.h"
#include "MyAIController.h"
#include "BossCharacter.h"
#include "EngineUtils.h"
#include "MinionAIController.generated.h"

UCLASS()
class PARTICLETEST_API AMinionAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	
	FVector currentVelocity;
	FVector acceleration;
	
	float distanceToPlayer;
	void MoveToPlayer();
	void RushToPlayer();

	void CheckNeighbours();

	AMyAIController* bossController;
	TArray<ABossCharacter*> bossCharacterList;

	void FindBossActor(UWorld* World, TArray<ABossCharacter*>& Out);
	void RandomizeAroundRadius();

	APawn* minionPawn;
	class AMinion* minionActor;
	ACharacter* playerCharacter;
	FVector directionToTarget;
	FVector targetPos;
	float decreaseFactor = 0.05f;

public:
	AMinionAIController();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float boidRadius = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float accelerationForce = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float maxMagnitude = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float repelForce = 0.1f;
};
