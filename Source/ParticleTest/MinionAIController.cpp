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
}

void AMinionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!minionActor)
	{
		minionPawn = GetPawn();
		minionActor = Cast<AMinion>(minionPawn);
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

	if (!minionActor->inAtkRadius)
	{
		MoveToPlayer();
	}
}

void AMinionAIController::MoveToPlayer()
{
	//MoveToLocation(playerCharacter->GetActorLocation(), 100.0f, false);
	minionActor->directionToPlayer = UKismetMathLibrary::GetDirectionUnitVector(minionActor->GetActorLocation(), playerCharacter->GetActorLocation());
	minionActor->directionToPlayer.Z = 0.0f;
	acceleration = minionActor->directionToPlayer * accelerationForce;
	acceleration.Z = 0.0f;

	CheckNeighbours();

	currentVelocity += acceleration;
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
	/*
	// Add
	for (int i = 0; i < GameManager.instance.boidList.Count; i++)
	{
		if (!neighBoursList.Contains(GameManager.instance.boidList[i]))
		{
			if (GameManager.instance.boidList[i] != this.gameObject)
			{
				if (Vector3.Distance(GameManager.instance.boidList[i].transform.position, transform.position) <= boidRadius)
				{
					neighBoursList.Add(GameManager.instance.boidList[i]);
				}
			}
		}
	}

	for (int i = 0; i < neighBoursList.Count; i++)
	{
		// Separation
		float distanceWithNeighbour = Vector3.Distance(neighBoursList[i].transform.position, transform.position);
		if (distanceWithNeighbour < crowdDistance)
		{
			acceleration += ((transform.position - neighBoursList[i].transform.position).normalized /
				distanceWithNeighbour * boidRadius * repelForce) * currentVelocity.magnitude;
		}

		// Remove
		if (Vector3.Distance(neighBoursList[i].transform.position, transform.position) > boidRadius)
		{
			neighBoursList.Remove(neighBoursList[i]);
		}
	}

	if (neighBoursList.Count > 0)
	{
		allignmentVector = allignmentVector / neighBoursList.Count;
		allignmentVector.z = 0f;
		acceleration += allignmentVector * accelerationForce;
	}
	*/
}


