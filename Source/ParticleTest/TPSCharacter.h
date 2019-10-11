// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h" 
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Minion.h"
#include "TPSCharacter.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAttackModeEnum : uint8
{
	
	EA_Heavy 	UMETA(DisplayName = "Heavy"),
	EA_Light	UMETA(DisplayName = "Light")
};

UCLASS()
class PARTICLETEST_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	ATPSCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EAttackModeEnum AttackTypeEnum= EAttackModeEnum::EA_Light;
	
	
	//UFUNCTION(BlueprintCallable, Category = "Something")
	//Refrence to main Character
	//Inputs
	UFUNCTION(BlueprintCallable,Category = "Player Movement")
	 void MoveForward(float value);
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	 void MoveRight(float value);
	UPROPERTY(BlueprintReadWrite ,Category = "Player Movement")
		bool CanMove = true;


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
		float Test;
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
	
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Lock On Properties")
		void LockOnFunction();
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
	
	
		float AngleDir(FVector fwd, FVector targetDir, FVector up);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lock On Properties")
		void SwitchDone();

	FName enemyTag;
	
	UPROPERTY(EditAnywhere, Category = "Animation Montages")
		TArray<UAnimMontage*>montages;

	UFUNCTION( BlueprintCallable, Category = "Melee Properties")
		void DoAttacks();
	UFUNCTION(BlueprintCallable, Category = "Melee Properties")
		void PlayerAttackedLight();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Melee Properties")
		void ComboAttackSave();
	UFUNCTION(BlueprintCallable, Category = "Melee Properties")
		void Dash();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Melee Properties")
		void ResetCombo();
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		int AttackCounts;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool inAttackAnimation;
	UPROPERTY(BlueprintReadWrite, Category = "Melee Properties")
		bool SaveAttack;
	
	
	






	ACharacter* GetGlobalPlayer();
	
	TArray<AMinion*> enemyList;

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
