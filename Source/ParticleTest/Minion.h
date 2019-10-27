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
	class AMinionAIController* minionAI;

public:
	// Sets default values for this character's properties
	AMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/** Destroy the actor */
	


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LookAtPlayer();
	void Initialize(bool f);

	bool inAtkRadius = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	bool isMelee = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	bool isFire = false;
	FVector directionToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	float minRadius;

	UFUNCTION(BlueprintCallable, Category = "Minion Behavior")
	void Kill();
};
