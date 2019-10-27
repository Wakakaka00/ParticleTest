// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BloodPool.generated.h"

UCLASS()
class PARTICLETEST_API ABloodPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloodPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blood Pool Properties")
		float destroySpeed = 1.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Effects")
		void SetOnFire();
};
