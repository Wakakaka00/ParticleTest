// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Minion.generated.h"

UCLASS()
class PARTICLETEST_API AMinion : public AEnemy
{
	GENERATED_BODY()

private:
	class AMinionAIController* minionAI;
	float atkResetDuration;
	float atkResetTimer = 0.0f;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		UCapsuleComponent* attackCollider;

	void LookAtPlayer();
	void Initialize(bool f);

	bool inAtkRadius = false;
	bool isAtk = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float damage = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	bool isMelee = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	bool isFire = false;
	FVector directionToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	float minRadius;

	UFUNCTION(BlueprintCallable, Category = "Minion Behavior")
	void Kill();

	void Attack();
};
