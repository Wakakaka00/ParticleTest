// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BloodProjectile.generated.h"

UCLASS()
class PARTICLETEST_API ABloodProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloodProjectile();
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Behavior")
	TSubclassOf<AActor> BloodPoolBlueprint;

	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
		int MyInt;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LaunchProjectile(float speed);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
