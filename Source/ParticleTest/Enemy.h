// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Enemy.generated.h"

UCLASS()
class PARTICLETEST_API AEnemy : public ACharacter
{
	GENERATED_BODY()



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	

public:
	
	// Sets default values for this character's properties
	AEnemy();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool isDamaged = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float currentHealth;

	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Lock On Properties")
	UStaticMeshComponent* sphereLooker;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		bool CanBeTargeted;
	UPROPERTY(BlueprintReadWrite)
		float MaximumDistance;
	UPROPERTY(BlueprintReadWrite)
		float ClosestTargetToMeDistance;


	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		AEnemy* ClosestTargetToMe;
	UPROPERTY(BlueprintReadWrite, Category = "Lock On Properties")
		float PlayerToEnemyDistance;
	UFUNCTION(BlueprintCallable, Category = "Lock On Properties")
		void Hi();
	UFUNCTION(BlueprintCallable, Category = "Lock On Properties")
		void SphereLookAt();

	UFUNCTION(BlueprintCallable, Category = "Damage")
		void SetIsDamaged(bool b);

	UFUNCTION(BlueprintCallable, Category = "Damage")
		bool GetIsDamaged();
	
	ACharacter* playerPawn;
	class ATPSCharacter* playerActor;
};
