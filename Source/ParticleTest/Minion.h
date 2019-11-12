// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Minion.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EnemyType : uint8
{
	NoType 	UMETA(DisplayName = "Notype"),
	Melee 	UMETA(DisplayName = "Melee"),
	Range	UMETA(DisplayName = "Range"),
	Fire	UMETA(DisplayName = "Fire")
};

UCLASS()
class PARTICLETEST_API AMinion : public AEnemy
{
	GENERATED_BODY()

private:
	class AMinionAIController* minionAI;
	float atkResetDuration;
	float atkResetTimer = 0.0f;
	float stunTimer = 0.0f;
	float stunDuration = 0.5f;
	float distanceBetweenPoint = 200.0f;

public:
	// Sets default values for this character's properties
	AMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	EnemyType enemyType;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		UCapsuleComponent* attackCollider;

	void LookAtPlayer();
	void Initialize(bool f);

	bool inAtkRadius = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool isAtk = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float damage = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	bool isMelee = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool isRoaming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	bool isFire = false;
	FVector directionToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	float minRadius;

	UFUNCTION(BlueprintCallable, Category = "Minion Behavior")
	void Kill();

	UFUNCTION(BlueprintImplementableEvent, Category = "Minion Behavior")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Minion Behavior")
	void DoDamage();

	UFUNCTION(BlueprintCallable, Category = "Minion Behavior")
	void Stun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	bool isStunned = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Minion Behavior")
	bool FindLineOfSight();

	UPROPERTY(EditAnywhere, Category = "Minion Behavior")
	TSubclassOf<AActor> ShootingPointBP;

	TArray<AActor*> shootingLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion Behavior")
	bool isShooting = false;

	void SpawnShootingPoint();
	void RepositionShootingPoint();
	UFUNCTION(BlueprintCallable, Category = "Minion Behavior")
	void DestroyShootingLine();
};
