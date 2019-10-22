// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"
#include "Engine/World.h"
#include "MinionAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values
AMinion::AMinion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	maxHealth = 100.0f;
	currentHealth = maxHealth;
}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();
	minionAI = Cast<AMinionAIController>(GetController());
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	LookAtPlayer();
	if (isFire)
	{
		currentHealth -= DeltaTime * (maxHealth / 18.0f);
		if (currentHealth <= 0.0f)
		{
			minionAI->bossController->FireMinionList.Remove(this);
			Destroy();
		}
	}
}

// Called to bind functionality to input
void AMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinion::Initialize(bool f)
{
	isFire = f;
	if (f)
	{
		minionAI->accelerationForce = 0.7f;
		minionAI->maxMagnitude = FMath::RandRange(5.8f,7.0f);
	}
}

void AMinion::LookAtPlayer()
{
	FVector playerLocation = playerPawn->GetActorLocation();
	playerLocation.Z = GetActorLocation().Z;
	directionToPlayer = playerLocation - GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(directionToPlayer).Rotator();
	SetActorRotation(Rot);
}

void AMinion::Kill()
{
	if(isFire) minionAI->bossController->FireMinionList.Remove(this);
	else minionAI->bossController->MinionList.Remove(this);

	Destroy();
}


