// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BloodProjectile.h"
#include "Minion.h"
#include "Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Portal.h"
#include "BossCharacter.generated.h"

UCLASS()
class PARTICLETEST_API ABossCharacter : public AEnemy
{
	GENERATED_BODY()

private:
	bool isLeft = false; // portal

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	ABossCharacter();
	USceneComponent* skeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	AActor* throne;

	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	TArray <APortal*> portalList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	APortal* targetPortal;
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
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
		void JumpDash(float DeltaSeconds);
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
		void CallBackYari();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable,Category = "Portal")
		void PortalDash();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "Boss Behavior")
		void JumpBackThrone();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	int dashCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	bool isPortalDash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	TArray<FHitResult> hitList;

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

	bool isForcePush = false;
	bool isJumpSlam = false;
	bool isDash = false;
	bool isBackJump = false;
	bool isNormalPush = false;
	bool isHealing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isDrinking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isJumpDash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isSpitting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isOnThrone = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	float pushBackForce = 0.0f;

	float spitDelay = 0.5f;
	FVector AimDirection = FVector(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Trigger Capsule")
	class UCapsuleComponent* PushCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isYariThrow = false;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void SetPushCollison(bool b);

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void FindNearestPortal();

	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Location")
	FVector playerLastPos;

	FVector handSocketLocation;

	int healCount = 0;
	int maxHealCount = 2;
};
