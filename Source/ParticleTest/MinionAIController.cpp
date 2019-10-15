// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

AMinionAIController::AMinionAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMinionAIController::BeginPlay()
{
	Super::BeginPlay();
	playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FindBossActor(GetWorld(), bossCharacterList);
	bossController = Cast<AMyAIController>(bossCharacterList[0]->GetController());
}

void AMinionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!minionActor)
	{
		minionPawn = GetPawn();
		minionActor = Cast<AMinion>(minionPawn);
		RandomizeAroundRadius();
	}
	
	if (playerCharacter && minionActor)
	{
		distanceToPlayer = FVector::Distance(minionActor->GetActorLocation(), playerCharacter->GetActorLocation());
		if (distanceToPlayer <= 150.0f)
		{
			minionActor->inAtkRadius = true;
		}
		else
		{
			if(minionActor->inAtkRadius == true)  minionActor->inAtkRadius = false;
		}
	}

	MoveToPlayer();	
}

void AMinionAIController::MoveToPlayer()
{
	if (bossController->nearestMinionList.Contains(minionActor))
	{
		if (!minionActor->inAtkRadius)
		{
			//MoveToLocation(playerCharacter->GetActorLocation(), 100.0f, false);
			minionActor->directionToPlayer = UKismetMathLibrary::GetDirectionUnitVector(minionActor->GetActorLocation(), playerCharacter->GetActorLocation());
			minionActor->directionToPlayer.Z = 0.0f;
			acceleration = minionActor->directionToPlayer * accelerationForce;
			acceleration.Z = 0.0f;
		}
		else
		{
			currentVelocity = FVector::ZeroVector;
			acceleration = FVector::ZeroVector;
		}
	}
	else if(bossController->nearestMinionList.Num() < 3)
	{
		minionActor->directionToPlayer = UKismetMathLibrary::GetDirectionUnitVector(minionActor->GetActorLocation(), playerCharacter->GetActorLocation());
		minionActor->directionToPlayer.Z = 0.0f;
		acceleration = minionActor->directionToPlayer * accelerationForce;
		acceleration.Z = 0.0f;
	}
	else if (minionActor->isMelee == false)
	{ 
		if (FVector::Distance(minionActor->GetActorLocation(),playerCharacter->GetActorLocation()) >= 720.0f)
		{
			if (currentVelocity != FVector::ZeroVector)
			{
				currentVelocity = FVector::ZeroVector;
				acceleration = FVector::ZeroVector;
				RandomizeAroundRadius();
			}	
		}
		else
		{
			acceleration = directionToTarget * accelerationForce;
			acceleration.Z = 0.0f;
		}
	}

	CheckNeighbours();

	currentVelocity += acceleration;
	currentVelocity = currentVelocity * 0.99f;
	if (currentVelocity.SizeSquared() > maxMagnitude * maxMagnitude)
	{
		FVector temp = currentVelocity;
		temp.GetSafeNormal(1.0f);
		temp.Normalize(1.0f);
		currentVelocity = temp * maxMagnitude;
	}

	FHitResult HitResult;
	minionActor->SetActorLocation(minionActor->GetActorLocation() + currentVelocity, false, &HitResult);

	minionActor->LookAtPlayer();
}

void AMinionAIController::CheckNeighbours()
{
	for (int i = 0; i < bossController->MinionList.Num(); i++)
	{
		// Separation between minions
		if (bossController->MinionList[i] != minionActor)
		{			 
			float distanceWithNeighbour = FVector::Distance(bossController->MinionList[i]->GetActorLocation(), minionActor->GetActorLocation());
			if (distanceWithNeighbour < 120.0f)
			{
				FVector direction = minionActor->GetActorLocation() - bossController->MinionList[i]->GetActorLocation();
				direction.GetSafeNormal(1.0f);
				direction.Normalize(1.0f);
				if (!minionActor->inAtkRadius)
				{
					acceleration += (direction /
						distanceWithNeighbour * boidRadius * repelForce) * currentVelocity.Size();
				}
				else
				{
					acceleration += ((direction - minionActor->GetActorForwardVector())/
						distanceWithNeighbour * boidRadius * (repelForce + 0.1f));
				}
			}
		}

		// Separation between boss
		float distanceToBoss = FVector::Distance(bossController->bossActor->GetActorLocation(), minionActor->GetActorLocation());
		if(distanceToBoss <= 180.0f)
		{
			FVector direction = minionActor->GetActorLocation() - bossController->bossActor->GetActorLocation();
			direction.GetSafeNormal(1.0f);
			direction.Normalize(1.0f);
			if (!minionActor->inAtkRadius)
			{
				acceleration += (direction /
					distanceToBoss * boidRadius * repelForce) * currentVelocity.Size();
			}
			else
			{

				acceleration += ((direction - minionActor->GetActorForwardVector()) /
					distanceToBoss * boidRadius * (repelForce + 0.1f));
			}
		}

		// Separtion between Player
		if (distanceToPlayer <= 120.0f)
		{
			FVector direction = minionActor->GetActorLocation() - playerCharacter->GetActorLocation();
			direction.GetSafeNormal(1.0f);
			direction.Normalize(1.0f);
	
			if (!minionActor->inAtkRadius)
			{
				acceleration += (direction /
					distanceToPlayer * boidRadius * repelForce) * currentVelocity.Size();
			}
			else
			{
				acceleration += (direction  /
					distanceToPlayer * boidRadius * (repelForce + 0.1f));
			}
		}
	}
}

void AMinionAIController::RandomizeAroundRadius()
{
	float randomXPos = minionActor->minRadius * cos(FMath::RandRange(0.0f, 360.0f));
	float randomYPos = minionActor->minRadius * sin(FMath::RandRange(0.0f, 360.0f));
	targetPos = playerCharacter->GetActorLocation() + FVector(randomXPos, randomYPos, 0.0f);
	targetPos.Z = minionActor->GetActorLocation().Z;
	directionToTarget = targetPos - minionActor->GetActorLocation();
	directionToTarget.GetSafeNormal(1.0f);
	directionToTarget.Normalize(1.0f);
}

void AMinionAIController::FindBossActor(UWorld* World, TArray<ABossCharacter*>& Out)
{
	for (TActorIterator<ABossCharacter> It(World); It; ++It)
	{
		Out.Add(*It);
	}
}


