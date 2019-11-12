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

	maxHealth = 100.0f;
	currentHealth = maxHealth;
}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();
	minionAI = Cast<AMinionAIController>(GetController());
	atkResetDuration = FMath::RandRange(3.0f, 5.0f);
	enemyType = EnemyType::NoType;
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	LookAtPlayer();
	if (enemyType != EnemyType::Fire && enemyType != EnemyType::Melee)
	{
		if (minionAI->bossController->nearestMinionList.Num() >= 3)
		{
			enemyType = EnemyType::Range;
		}
		else
		{
			enemyType = EnemyType::NoType;
		}
	}	

	/*if (isFire)
	{
		currentHealth -= DeltaTime * (maxHealth / 18.0f);
		if (currentHealth <= 0.0f)
		{
			minionAI->bossController->FireMinionList.Remove(this);
			Destroy();
		}
	}*/

	if (isAtk)
	{
		atkResetTimer += DeltaTime;
		if (atkResetTimer >= atkResetDuration)
		{
			atkResetTimer = 0.0f;
			atkResetDuration = FMath::RandRange(3.0f, 5.0f);
			isAtk = false;
			isRoaming = false;
		}
	}

	if (isStunned)
	{
		stunTimer += DeltaTime;
		if (stunTimer >= stunDuration)
		{
			stunTimer = 0.0f;
			isStunned = false;
		}
	}

	//FVector d = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), playerPawn ->GetActorLocation());
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + d * 400.0f, FColor::Green, false, 1, 0, 2);
}

// Called to bind functionality to input
void AMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinion::Initialize(bool f)
{
	if (f)
	{
		minionAI->accelerationForce = 0.7f;
		minionAI->maxMagnitude = FMath::RandRange(5.8f,7.0f);
		enemyType = EnemyType::Fire;
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
	if(enemyType == EnemyType::Fire) minionAI->bossController->FireMinionList.Remove(this);
	else
	{
		if (minionAI->bossController->nearestMinionList.Contains(this))
		{
			minionAI->bossController->nearestMinionList.Remove(this);
		}
		minionAI->bossController->MinionList.Remove(this);
	}

	Destroy();
}

void AMinion::Stun()
{
	stunTimer = 0.0f;
	if (!isStunned) isStunned = true;
	minionAI->currentVelocity = FVector::ZeroVector;
	minionAI->acceleration = FVector::ZeroVector;
}

void AMinion::DoDamage()
{
	attackCollider->SetCollisionProfileName(TEXT("Trigger"));
}

void AMinion::SpawnShootingPoint()
{
	if (shootingLine.Num() == 0)
	{
		if (FVector::Distance(GetActorLocation(), playerPawn->GetActorLocation()) > distanceBetweenPoint)
		{
			auto point = GetWorld()->SpawnActor<AActor>(ShootingPointBP->GetOwnerClass(), GetActorLocation(), FRotator::ZeroRotator);
			shootingLine.Add(point);
		}
	}
	else if (FVector::Distance(shootingLine.Last()->GetActorLocation(), playerPawn->GetActorLocation()) > distanceBetweenPoint)
	{
		auto point = GetWorld()->SpawnActor<AActor>(ShootingPointBP->GetOwnerClass(), GetActorLocation(), FRotator::ZeroRotator);
		shootingLine.Add(point);
	}

	if (shootingLine.Num() >= 2)
	{
		if (FVector::Distance(shootingLine[shootingLine.Num() - 2]->GetActorLocation(), playerPawn->GetActorLocation()) < distanceBetweenPoint)
		{
			auto point = shootingLine[shootingLine.Num() - 1];
			shootingLine.RemoveAt(shootingLine.Num() - 1);
			point->Destroy();
		}
	}	
}

void AMinion::RepositionShootingPoint()
{
	for (int i = 0; i < shootingLine.Num(); i++)
	{
		FVector location = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), playerPawn->GetActorLocation());
		location = GetActorLocation() + location * (distanceBetweenPoint * (i +1));
		shootingLine[i]->SetActorLocation(location);
	}
}


