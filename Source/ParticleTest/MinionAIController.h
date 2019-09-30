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
	
	
	float distanceToPlayer;

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
