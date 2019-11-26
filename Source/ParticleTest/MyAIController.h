// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Yari.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyAIController.generated.h"

UCLASS()
class PARTICLETEST_API AMyAIController : public AAIController
{
	GENERATED_BODY()

	private:
	
		APawn* bossPawn;
		FVector playerLastPos;
		float bossLastDistance;
		FVector backJumpLocation;
		FVector playerForcePushLocation;

		void JumpSlamDashLerp(float DeltaSeconds);
		void BackJump(float DeltaSeconds);
		void ForcePush(float DeltaSeconds);
		void NormalPush(float DeltaSeconds);	

		float bloodPoolRadius = 20.0f;
		int currentHealAmount = 0;
		float healTimer = 0.0f;
		int maxHealAmount = 3;
		float drinkBloodDuration = 0.0f;
		float drinkBloodTimer = 0.0f;
		float spitTimer = 0.0f;
		float yariThrowTimer = 0.0f;
		float minionMaxDistance = 1000.0f;
		int updateDelay = 0;

		float findPlayerTimer = 0.0f;
		float findPlayerDuration = 0.5f;

		FVector directionMoving;
		FVector acceleration;
		FVector currentVelocity;

		void CheckNearestEnemy();
		void DashToPortalOrPlayer(float DeltaSeconds);
		void LookAtPortal();
		void LookAtVelocity();

		float recoveryTimer = 0.0f;
		float recoveryDuration = 0.0f;
		void RandomizeRecovery();

		TArray<AttackType> AttackList;

	public:
		AMyAIController();
		
		virtual void BeginPlay() override;

		virtual void Tick(float DeltaSeconds) override;

		ACharacter* playerCharacter;
		FVector playerLocation;
		class ABossCharacter* bossActor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class AYari* yari;

		void OnTooFar();		

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
		float backJumpDistance = 0.0f;			

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		float startYariThrowDuration = 20.0f;

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void RecoverFromStun();

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void SpitBlood();

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void SummonMinion();

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void ThrowYari();

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void CallBackYari();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		TArray<AMinion*> MinionList;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		TArray<AMinion*> FireMinionList;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		TArray<AMinion*> nearestMinionList;

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
		void AddMinion(AMinion * minion);

		void AddFireMinion(AMinion * minion);

		FTimerHandle SpitTimer;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
		FVector bossLastPos; //move back to Private after test

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
			float accelerationForce;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
			float maxMagnitude;

		UFUNCTION(BlueprintCallable, Category = "Boss Behavior")
			void ResetVelocity();

		UFUNCTION(BlueprintCallable, Category = "Portal")
		void FindPortalDirection();

		UFUNCTION(BlueprintCallable, Category = "Portal")
		void FindPlayerDirection();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
			bool isEnterArena = false;

};
