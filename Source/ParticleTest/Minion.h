// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Minion.generated.h"

UCLASS()
class PARTICLETEST_API AMinion : public AEnemy
{
	GENERATED_BODY()

private:
	

public:
	// Sets default values for this character's properties
	AMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LookAtPlayer();
	bool inAtkRadius = false;
	FVector directionToPlayer;

	TArray<AEnemy*> neibourList;
};
