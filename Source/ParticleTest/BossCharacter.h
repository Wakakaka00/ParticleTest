// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "BloodProjectile.h"
#include "Minion.h"
#include "Enemy.h"
#include "BossCharacter.generated.h"

UCLASS()
class PARTICLETEST_API ABossCharacter : public AEnemy
{
	GENERATED_BODY()

private:
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	ABossCharacter();
	USceneComponent* skeletalMesh;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LookAtPlayer();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
		void JumpSlam();
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
		void Dash();
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
		void ForcePush();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool isAtk = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	float distance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	float farDistance = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	float stopDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int damageTaken = 0;

	UPROPERTY(EditAnywhere, Category = "Boss Behavior")
	TSubclassOf<ABloodProjectile> BloodProjectileBlueprint;

	UPROPERTY(EditAnywhere, Category = "Boss Behavior")
	TSubclassOf<AMinion> MinionBlueprint;

	void RotateBlood();

	UPROPERTY(EditAnywhere, Category = "Boss Behavior")
	float bloodLaunchSpeed = 2000;

	UPROPERTY(EditAnywhere, Category = "Boss Behavior")
	float yariLaunchSpeed = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	TArray<AActor*> bloodPoolList;

	ACharacter* playerCharacter;

	bool isForcePush = false;
	bool isJumpSlam = false;
	bool isDash = false;
	bool isBackJump = false;
	bool isNormalPush = false;
	bool isHealing = false;
	bool isDrinking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool isJumpDash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isSpitting = false;

	float spitDelay = 0.3f;
	FVector AimDirection = FVector(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Trigger Capsule")
	class UCapsuleComponent* PushCapsule;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void SetPushCollison(bool b);

	FVector playerLastPos; // temp

	FVector handSocketLocation;

	float currentHealth;
	float maxHealth;
	int healCount = 0;
	int maxHealCount = 2;
};
