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

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class BossPositionState : uint8
{
	OnThrone 	UMETA(DisplayName = "OnThrone"),
	ThroneDash 	UMETA(DisplayName = "ThroneDash"),
	JumpToThrone	UMETA(DisplayName = "JumpToThrone"),
	OnGround	UMETA(DisplayName = "OnGround")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class BossPhase : uint8
{
	Phase1 	UMETA(DisplayName = "Phase1"),
	Phase2 	UMETA(DisplayName = "Phase2"),
	Phase3	UMETA(DisplayName = "Phase3")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class BossState : uint8
{
	Recovery 	UMETA(DisplayName = "Recovery"),
	Break 	UMETA(DisplayName = "Break"),
	Vulnerable	UMETA(DisplayName = "Vulnerable"),
	Attack UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class AttackType : uint8
{
	Lunge 	UMETA(DisplayName = "Lunge"),
	Arc5 	UMETA(DisplayName = "Arc5"),
	Arc3	UMETA(DisplayName = "Arc3"),
	AOE UMETA(DisplayName = "AOE")
};

UCLASS()
class PARTICLETEST_API ABossCharacter : public AEnemy
{
	GENERATED_BODY()

private:
	bool isLeft = false; // portal
	float breakTimer = 0.0f;
	float vulnerableTimer = 0.0f;
	float vulnerableDuration = 4.0f;
	bool isTracking = false;
	class AMyAIController* aiController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	ABossCharacter();
	USceneComponent* skeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	AActor* throne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		BossPositionState bossPositionState;

	BossPhase bossPhase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss State")
	BossState bossState;

	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	TArray <APortal*> portalList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	APortal* targetPortal;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Look At Player")
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Boss Behavior")
	float yariLaunchSpeed = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	TArray<AActor*> bloodPoolList;

	bool isForcePush = false;
	bool isJumpSlam = false;
	bool isDash = false;
	bool isBackJump = false;
	bool isNormalPush = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	bool isStart = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float atkDistance;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void SetPushCollison(bool b);

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void FindNearestPortal();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void Break();

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void SetVulnerable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Location")
	FVector playerLastPos;

	FVector handSocketLocation = FVector::ZeroVector;

	int healCount = 0;
	int maxHealCount = 2;

	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	bool isAttackingVital();


	// Animations
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayDrinking();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayVulnerable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayArc3Hit();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations")
		void PlayDefenseBreak();

	UFUNCTION(BlueprintCallable, Category = "Animations")
		void ResetAttack();

	UFUNCTION(BlueprintCallable, Category = "Animations")
		void TrackingPlayer();

	UFUNCTION(BlueprintCallable, Category = "Animations")
		void StopTrackingPlayer();

	bool GetIsTracking();
};
