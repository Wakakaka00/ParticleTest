// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BossCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Yari.generated.h"

UCLASS()
class PARTICLETEST_API AYari : public AActor
{
	GENERATED_BODY()
	
private:	
	// Sets default values for this actor's properties
	void StopYari();
	FTimerHandle StopTimer;
	float gravityScale;
	FVector initLocation;
	FRotator initRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AYari();
	UProjectileMovementComponent* ProjectileMovement;
	USceneComponent* SceneRoot = nullptr;
	ABossCharacter* bossActor;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Box")
		class UBoxComponent* TriggerBox;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrowOnGround(float speed, FRotator angle);
	void BackToHandSocket(float DeltaSeconds);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool isThrowing = false;
	bool isBack = false;

	
};
