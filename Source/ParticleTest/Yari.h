// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "BossCharacter.h"
#include "TPSCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Yari.generated.h"

UCLASS()
class PARTICLETEST_API AYari : public AStaticMeshActor
{
	GENERATED_BODY()
	
private:	
	// Sets default values for this actor's properties
	void StopYari();
	FTimerHandle StopTimer;
	float gravityScale;
	FVector initLocation;
	FRotator initRotation;
	ACharacter* playerCharacter;
	ATPSCharacter* playerActor;
	class AMyAIController* bossAI;
	bool isDamaged = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AYari();
	UProjectileMovementComponent* ProjectileMovement;
	USceneComponent* SceneRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	ABossCharacter* bossActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Box")
		class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrowOnGround(float speed, FRotator angle);
	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
	void ThrowOnThrone();

	void BackToHandSocket(float DeltaSeconds);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapPlayer(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	bool isThrowing = false;

	bool isBack = false;
	bool isFire = true;

	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Behavior")
	void DamagePlayer();

	UFUNCTION(BlueprintCallable, Category = "Pos")
	void SetInitPos(FVector location, FRotator rot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radius")
		float pushBackRadius;

	void SetYariCollision(bool b);
};
