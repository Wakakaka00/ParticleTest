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
	
	if (minionActor->isStunned)
	{
		return;
	}

	if (playerCharacter && minionActor)
	{			
		distanceToPlayer = FVector::Distance(minionActor->GetActorLocation(), playerCharacter->GetActorLocation());
		if (distanceToPlayer <= 120.0f)
		{
			minionActor->inAtkRadius = true;
		}
		else
		{
			if (minionActor->inAtkRadius == true)  minionActor->inAtkRadius = false;
		}
		if (!minionActor->isRoaming && !minionActor->isAtk)
		{
			MoveToPlayer();
			angle = 0.0f;
		}
		else if(minionActor->isRoaming)
		{
			Roaming();
		}

		currentVelocity += acceleration;
		currentVelocity = currentVelocity * 0.99f;
		if (minionActor->isRoaming)
		{
			if (currentVelocity.SizeSquared() > 2.2f * 2.2f)
			{
				FVector temp = currentVelocity;
				temp.GetSafeNormal(1.0f);
				temp.Normalize(1.0f);
				currentVelocity = temp * 2.2f;
			}
		}
		else
		{
			if (currentVelocity.SizeSquared() > maxMagnitude * maxMagnitude)
			{
				FVector temp = currentVelocity;
				temp.GetSafeNormal(1.0f);
				temp.Normalize(1.0f);
				currentVelocity = temp * maxMagnitude;
			}
		}	


		FHitResult HitResult;
		minionActor->SetActorLocation(minionActor->GetActorLocation() + currentVelocity, true, &HitResult);

		minionActor->LookAtPlayer();
	}

	if (minionActor->enemyType == EnemyType::Melee || minionActor->enemyType == EnemyType::Fire)
	{
		if (minionActor->inAtkRadius && !minionActor->isAtk)
		{
			minionActor->Attack();
			currentVelocity = FVector::ZeroVector;
			acceleration = FVector::ZeroVector;
		}
	}
	else if(minionActor->enemyType == EnemyType::Range)
	{
		if (FVector::Distance(minionActor->GetActorLocation(), playerCharacter->GetActorLocation()) >= 1300.0f && !minionActor->isAtk)
		{	
			findLineDelay += 1;
			if (findLineDelay >= 5)
			{
				findLineDelay = 0;
				if (minionActor->FindLineOfSight())
				{
					currentVelocity = FVector::ZeroVector;
					acceleration = FVector::ZeroVector;
					minionActor->isRoaming = false;
					minionActor->Attack();
					roamLeft = FMath::RandRange(0, 2);
				}
				else
				{
					if (!minionActor->isRoaming) minionActor->isRoaming = true;
				}
			}			
		}
	}

	// Range attack line of sight points
	if (minionActor->enemyType == EnemyType::Range && minionActor->isAtk)
	{
		//minionActor->SpawnShootingPoint();
	}
}

void AMinionAIController::MoveToPlayer()
{
	if (minionActor->enemyType != EnemyType::Fire)
	{
		if (minionActor->enemyType == EnemyType::Melee)
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
				if (!isAvoiding)
				{
					currentVelocity = FVector::ZeroVector;
					acceleration = FVector::ZeroVector;
				}	
			}
		}
		else if (minionActor->enemyType == EnemyType::NoType)
		{
			minionActor->directionToPlayer = UKismetMathLibrary::GetDirectionUnitVector(minionActor->GetActorLocation(), playerCharacter->GetActorLocation());
			minionActor->directionToPlayer.Z = 0.0f;
			acceleration = minionActor->directionToPlayer * accelerationForce;
			acceleration.Z = 0.0f;
		}
		else if (minionActor->enemyType == EnemyType::Range)
		{
			float distance = FVector::Distance(minionActor->GetActorLocation(), playerCharacter->GetActorLocation());
			if (distance >= 1300.0f && distance <= 1600.0f)
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
	}
	else
	{
		if (!minionActor->isRoaming)
		{
			if (!minionActor->inAtkRadius)
			{
				minionActor->directionToPlayer = UKismetMathLibrary::GetDirectionUnitVector(minionActor->GetActorLocation(), playerCharacter->GetActorLocation());
				minionActor->directionToPlayer.Z = 0.0f;
				acceleration = minionActor->directionToPlayer * accelerationForce;
				acceleration.Z = 0.0f;
			}
			else
			{
				if (!isAvoiding)
				{
					currentVelocity = FVector::ZeroVector;
					acceleration = FVector::ZeroVector;
				}
			}		
		}		
	}

	Avoidance();
}

void AMinionAIController::Avoidance()
{
	isAvoiding = false;
	// Separation between minions
	for (int i = 0; i < bossController->MinionList.Num(); i++)
	{		
		if (bossController->MinionList[i] != minionActor)
		{
			float distanceWithNeighbour = FVector::Distance(bossController->MinionList[i]->GetActorLocation(), minionActor->GetActorLocation());
			if (distanceWithNeighbour < boidRadius)
			{
				direction = minionActor->GetActorLocation() - bossController->MinionList[i]->GetActorLocation();
				direction.GetSafeNormal(1.0f);
				direction.Normalize(1.0f);
				if (!minionActor->inAtkRadius)
				{
					acceleration += (direction /
						distanceWithNeighbour * boidRadius * repelForce * decreaseFactor) * currentVelocity.Size();
				}
				else
				{
					acceleration += (direction /
						distanceWithNeighbour * boidRadius *(repelForce - 0.1f)* decreaseFactor);
				}
			}
		}
	}

	for (int i = 0; i < bossController->FireMinionList.Num(); i++)
	{
		if (bossController->FireMinionList[i] != minionActor)
		{
			float distanceWithNeighbour = FVector::Distance(bossController->FireMinionList[i]->GetActorLocation(), minionActor->GetActorLocation());
			if (distanceWithNeighbour < boidRadius)
			{
				direction = minionActor->GetActorLocation() - bossController->FireMinionList[i]->GetActorLocation();
				direction.GetSafeNormal(1.0f);
				direction.Normalize(1.0f);
				if (!minionActor->inAtkRadius)
				{
					acceleration += (direction /
						distanceWithNeighbour * boidRadius * repelForce * decreaseFactor) * currentVelocity.Size();
				}
				else
				{
					acceleration += (direction /
						distanceWithNeighbour * boidRadius * (repelForce - 0.1f)* decreaseFactor);
				}
			}
		}
	}

	// Separation between boss
	float distanceToBoss = FVector::Distance(bossController->bossActor->GetActorLocation(), minionActor->GetActorLocation());
	if(distanceToBoss <= bossAvoidDistance)
	{
		direction = minionActor->GetActorLocation() - bossController->bossActor->GetActorLocation();
		direction.GetSafeNormal(1.0f);
		direction.Normalize(1.0f);
		if (!minionActor->inAtkRadius)
		{
			acceleration += (direction /
				distanceToBoss * bossAvoidDistance * repelForce * decreaseFactor) * currentVelocity.Size();
		}
		else
		{
			acceleration += (direction /
			distanceToBoss * bossAvoidDistance * (repelForce - 0.1f) * decreaseFactor);
		}
	}

	// Separation between Player	
	if (distanceToPlayer < playerAvoidDistance)
	{
		direction = minionActor->GetActorLocation() - playerCharacter->GetActorLocation();
		direction.GetSafeNormal(1.0f);
		direction.Normalize(1.0f);
		isAvoiding = true;
		if (!minionActor->inAtkRadius)
		{
			acceleration += (direction /
			distanceToPlayer * playerAvoidDistance * repelForce * decreaseFactor) * currentVelocity.Size();
		}
		else
		{
			acceleration += (direction  /
			distanceToPlayer * playerAvoidDistance * repelForce * decreaseFactor);
		}
	}

	// Separation between shootingLine
	for (int i = 0; i < bossController->MinionList.Num(); i++)
	{
		for (int j = 0; j < bossController->MinionList[i]->shootingLine.Num(); j++)
		{	
			FVector point = bossController->MinionList[i]->shootingLine[j]->GetActorLocation();
			float distanceWithLine = FVector::Distance(point, minionActor->GetActorLocation());
			if (distanceWithLine < boidRadius)
			{
				direction = minionActor->GetActorLocation() - point;
				direction.GetSafeNormal(1.0f);
				direction.Normalize(1.0f);
				if (!minionActor->inAtkRadius)
				{
					acceleration += (direction /
						distanceWithLine * boidRadius * repelForce * decreaseFactor) * currentVelocity.Size();
				}
				else
				{
					acceleration += (direction /
						distanceWithLine * boidRadius *(repelForce - 0.1f)* decreaseFactor);
				}
			}
		}		
	}
}

void AMinionAIController::Roaming()
{
	if (minionActor->enemyType == EnemyType::Melee || minionActor->enemyType == EnemyType::Fire)
	{
		direction = -(minionActor->GetActorForwardVector());
		isAvoiding = true;
		acceleration += direction * (accelerationForce * decreaseFactor);

		// Separation between boss
		float distanceToBoss = FVector::Distance(bossController->bossActor->GetActorLocation(), minionActor->GetActorLocation());
		if (distanceToBoss <= bossAvoidDistance)
		{
			direction = minionActor->GetActorLocation() - bossController->bossActor->GetActorLocation();
			direction.GetSafeNormal(1.0f);
			direction.Normalize(1.0f);

			acceleration += (direction /
				distanceToBoss * bossAvoidDistance * (repelForce - 0.1f) * decreaseFactor);
		}

		// Separtion between Player	
		if (distanceToPlayer < playerAvoidDistance)
		{
			direction = minionActor->GetActorLocation() - playerCharacter->GetActorLocation();
			direction.GetSafeNormal(1.0f);
			direction.Normalize(1.0f);
			if (!minionActor->inAtkRadius)
			{
				acceleration += (direction /
					distanceToPlayer * playerAvoidDistance * repelForce * decreaseFactor) * currentVelocity.Size();
			}
			else
			{
				acceleration += (direction /
					distanceToPlayer * playerAvoidDistance * repelForce * decreaseFactor);
			}
		}
	}
	else if(minionActor->enemyType == EnemyType::Range)
	{	
		if (angle == 0.0f)
		{
			acceleration = FVector::ZeroVector;
			currentVelocity = FVector::ZeroVector;
		}
		
		if (angle >= 360.0f || angle <= -360.0f)
		{
			angle = 0.001f;
		}

		if(!roamLeft) angle += 0.001f;
		else angle -= 0.001f;
		float x = distanceToPlayer * sin(angle);
		float y = distanceToPlayer * cos(angle);

		FVector nextPos = FVector(x + playerCharacter->GetActorLocation().X, y + playerCharacter->GetActorLocation().Y, minionActor->GetActorLocation().Z);
		direction = UKismetMathLibrary::GetDirectionUnitVector(minionActor->GetActorLocation(), nextPos);

		acceleration += direction * accelerationForce;

		Avoidance();
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


