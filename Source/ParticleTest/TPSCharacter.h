// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "Engine.h"
#include "Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h" 
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Minion.h"
#include "BossCharacter.h"
#include "TPSCharacter.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESwitchModeEnum : uint8
{
	
	ES_Heavy 	UMETA(DisplayName = "Heavy"),
	ES_Light	UMETA(DisplayName = "Light")
};

UCLASS()
class PARTICLETEST_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	float pushBackTimer = 0.0f;

public:
	// Sets default values for this character's properties
	ATPSCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		ESwitchModeEnum SwitchMode= ESwitchModeEnum::ES_Light;
	
	
	//UFUNCTION(BlueprintCallable, Category = "Something")
	//Refrence to main Character
	//Inputs
	UFUNCTION(BlueprintCallable,Category = "Player Movement")
	 void MoveForward(float value);
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	 void MoveRight(float value);
	UPROPERTY(BlueprintReadWrite ,Category = "Player Movement")
		bool CanMove = true;
	UPROPERTY(BlueprintReadWrite, Category = "Player Movement")
		FVector targetForwardVector;
	UPROPERTY(BlueprintReadWrite, Category = "Player Movement")
		FVector targetRightVector;
	UPROPERTY(BlueprintReadWrite, Category = "Player Movement")
		FVector desiredDir;
	UPROPERTY(BlueprintReadWrite, Category = "Player Movement")
		int DirectionsHolden;
	UPROPERTY(BlueprintReadWrite, Category = "Player Movement")
		bool PlayerHoldingAOrD=false ;
	UPROPERTY(BlueprintReadWrite, Category = "Player Movement")
		bool PlayerHoldingW = false;

	UPROPERTY(BlueprintReadWrite, Category = "Health Properties")
		float MaximumHealth;
	UPROPERTY(BlueprintReadWrite, Category = "Health Properties")
		float CurrentHealth;
	UPROPERTY(BlueprintReadWrite, Category = "Health Properties")
		float MinimumHealth;


	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		bool TargetLocked;	
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		bool FoundNewTarget;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		float ClosestTargetDistance;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		float ClosestTargetDistanceToTargetedEnemy;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		float MaximumDistance;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		float ClosestTargetRotation;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		float MouseMoveAngle;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		bool ShouldRotateLockOn;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		UCameraComponent* camera;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		USphereComponent* lockCollision;

	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		FVector StartVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On Properties")
		AEnemy* NearestTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On Properties")
		AEnemy* EnemyElement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On Properties")
		AEnemy* NewTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
		ABossCharacter* bossActor;
	
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Lock On Properties")
		void LockOnFunction(bool OnlyBoss);
	UFUNCTION(BlueprintNativeEvent, Category = "Lock On Properties")
		void LockOnDone();
	UFUNCTION(BlueprintNativeEvent, Category = "Lock On Properties")
		void SetLockOnToTarget();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Lock On Properties")
		void SwitchLeft();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Lock On Properties")
		void SwitchRight();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lock On Properties")
		void SwitchUp();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lock On Properties")
		void SwitchDown();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lock On Properties")
		void SwitchToBoss();
	
	
		float AngleDir(FVector fwd, FVector targetDir, FVector up);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lock On Properties")
		void SwitchDone();

	FName enemyTag;
	
	UPROPERTY(EditAnywhere, Category = "Animation Montages")
		TArray<UAnimMontage*>montages;

	UFUNCTION( BlueprintCallable, Category = "Melee Properties")
		void DoAttacks(bool DoCharge);
	UFUNCTION(BlueprintCallable, Category = "Melee Properties")
		void PlayerAttackedLight();
	UFUNCTION(BlueprintCallable, Category = "Melee Properties")
		void Dash();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Melee Properties")
		void ResetCombo();
	UFUNCTION(BlueprintCallable, Category = "Melee Properties")
		void ContinueCombo();
	

	UFUNCTION( BlueprintCallable, Category = "Melee Properties")
		void SetCanContinueCombo(bool b);
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		int AttackCounts;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		int HeavyChargeAttackCounterOrder;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool inAttackAnimation;

	UPROPERTY(BlueprintReadWrite, Category = "Evade Properties")
		bool CanEvade = true;
	

	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool CanContinueCombo = false;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool PostCanContinueCombo = false;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool ReservedChargedAttack = false;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool CanAttack = true;
	
	UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
	void PushBack(float force, FVector pushLocation);

	ACharacter* GetGlobalPlayer();

	bool isPushingBack = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Movement(FVector direction, float v);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void BindInputs();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

};
