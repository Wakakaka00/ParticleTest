// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Minion.h"
#include "Runtime/Core/Public/Math/Vector.h"


AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	bossPawn = GetPawn();
	bossActor = Cast<ABossCharacter>(bossPawn);
}

void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!isEnterArena)
	{
		return;
	}

	bossActor->distance = FVector::Distance(bossPawn->GetActorLocation(), playerLocation);
	CheckNearestEnemy();

	if (!bossActor->isStart)
	{
		if (bossActor->bossState == BossState::Recovery)
		{
			if (!bossActor->isAtk)
			{
				playerLocation = playerCharacter->GetActorLocation();
				MoveToLocation(playerLocation, 200.0f, false);
				// Too Far 
				/*if (bossActor->distance >= bossActor->farDistance)
				{
					UE_LOG(LogTemp, Warning, TEXT("Far"));
					bossActor->isAtk = true;
					OnTooFar();
				}
		*/
				if (bossActor->damageTaken >= 4)
				{
					bossActor->isAtk = true;
					bossLastPos = bossActor->GetActorLocation();
					bossLastDistance = bossActor->distance;
					bossActor->isBackJump = true;
				}

				if (bossActor->currentHealth < bossActor->maxHealth * 0.3f && bossActor->healCount < bossActor->maxHealCount)
				{
					bossActor->PlayDrinking();
					bossActor->isAtk = true;
					bossActor->isHealing = true;
					bossActor->healCount += 1;
				}

				/*if (bossActor->bloodPoolList.Num() < 3)
				{
					bossActor->isAtk = true;
					bossActor->isDrinking = true;
				}*/
			}

			// JumpSlam / Dash
			if (bossActor->isJumpSlam || bossActor->isDash)
			{
				JumpSlamDashLerp(DeltaSeconds);
			}

			// Back Jump
			if (bossActor->isBackJump)
			{
				BackJump(DeltaSeconds);
			}

			// Force Push
			if (bossActor->isForcePush)
			{
				ForcePush(DeltaSeconds);
			}

			// Normal Push
			if (bossActor->isNormalPush)
			{
				playerLastPos = bossActor->playerLastPos;
				NormalPush(DeltaSeconds);
			}

			// Spit blood
			if (bossActor->isSpitting)
			{
				bossActor->LookAtPlayer();
			}

			// Heal Guord
			if (bossActor->isHealing)
			{
				StopMovement();
				bossActor->LookAtPlayer();
				//currentHealAmount += 3.0f * DeltaSeconds;
				//bossActor->currentHealth += 3.0f * DeltaSeconds;
				healTimer += DeltaSeconds;
				if (healTimer >= 2.0f)
				{
					currentHealAmount += 1;
					bossActor->currentHealth += (bossActor->maxHealth * 0.1f);
					healTimer = 0.0f;
				}
				if (currentHealAmount >= maxHealAmount)
				{
					bossActor->isAtk = false;
					bossActor->isHealing = false;
					currentHealAmount = 0.0f;
					healTimer = 0.0f;
				}
			}

			// Drink Blood
			if (bossActor->isDrinking)
			{
				drinkBloodTimer += DeltaSeconds;
				if (drinkBloodTimer >= 1.0f)
				{
					drinkBloodDuration += 1.0f;
					drinkBloodTimer = 0.0f;
				}
				if (drinkBloodDuration >= 3.0f)
				{
					bossActor->isDrinking = false;
					bossActor->isAtk = false;
				}
			}

			// Jump Dash from throne
			if (bossActor->isJumpDash)
			{
				bossActor->JumpDash(DeltaSeconds);
				bossActor->isOnThrone = false; // Move it not tick
			}

			// Portal Dash
			if (bossActor->isPortalDash)
			{
				DashToPortalOrPlayer(DeltaSeconds);
			}
		}
		else
		{
			StopMovement();
		}
	}
	else // isStart
	{
		bossActor->LookAtPlayer();
		yariThrowTimer += DeltaSeconds;
		if (yariThrowTimer >= startYariThrowDuration)
		{
			if (!bossActor->isYariThrow)
			{
				yariThrowTimer = 0.0f;
				bossActor->isStart = false;
				bossActor->GetCharacterMovement()->GravityScale = 0.0f;
				StopMovement();
				bossActor->isAtk = true;
				bossLastPos = bossActor->GetActorLocation();
				bossActor->isJumpDash = true;
			}
		}
	}
}

void AMyAIController::OnTooFar()
{
	/*int randNum = FMath::RandRange(0, 1);
	if (randNum == 0)
	{
		bossActor->JumpSlam();
		bossActor->isJumpSlam = true;
	}
	else
	{
		bossActor->Dash();
		bossActor->isDash = true;
	}
	*/
	bossActor->JumpSlam();
	bossActor->isJumpSlam = true;
	playerLastPos = playerLocation;
}

void AMyAIController::JumpSlamDashLerp(float DeltaSeconds)
{
	FVector tempLocation = FMath::Lerp(bossActor->GetActorLocation(), playerLastPos, DeltaSeconds * 4.0f);
	FHitResult HitResult;
	bossActor->SetActorLocation(tempLocation, false, &HitResult);

	if (FVector::Distance(bossActor->GetActorLocation(), playerLastPos) <= bossActor->stopDistance)
	{
		bossActor->isAtk = false;
		if(bossActor->isJumpSlam) bossActor->isJumpSlam = false;
		else if (bossActor->isDash) bossActor->isDash = false;
	}
}

void AMyAIController::BackJump(float DeltaSeconds)
{
	backJumpLocation = bossLastPos + (-bossActor->GetActorForwardVector()* (1000.0f - bossLastDistance));
	FVector tempLocation = FMath::Lerp(bossActor->GetActorLocation(), backJumpLocation, DeltaSeconds * 4.0f);

	FHitResult HitResult;
	bossActor->SetActorLocation(tempLocation, false, &HitResult);
	
	backJumpDistance = FVector::Distance(bossActor->GetActorLocation(), backJumpLocation);
	if (backJumpDistance <= 50.0f)
	{
		bossActor->isAtk = false;
		bossActor->damageTaken = 0;
		bossActor->isBackJump = false;
	}
}

void AMyAIController::RecoverFromStun()
{
	bossActor->isForcePush = true;
	bossActor->isAtk = true;
	playerLastPos = playerLocation;
}

void AMyAIController::ForcePush(float DeltaSeconds)
{
	if (bossActor->distance <= 700.0f)
	{
		FVector direction = UKismetMathLibrary::GetDirectionUnitVector(playerLocation,bossActor->GetActorLocation());
		direction.Z = 0.0f;
		playerForcePushLocation = playerLastPos + (-direction * 800.0f);
		FVector tempLocation = FMath::Lerp(playerCharacter->GetActorLocation(), playerForcePushLocation, DeltaSeconds * 4.0f);

		FHitResult HitResult;
		playerCharacter->SetActorLocation(tempLocation, false, &HitResult);

		if (FVector::Distance(playerCharacter->GetActorLocation(), playerForcePushLocation) <= 45.0f) // or if hit wall or minion
		{
			bossActor->isAtk = false;
			bossActor->isForcePush = false;
		}
	}
}

void AMyAIController::NormalPush(float DeltaSeconds)
{
	if (bossActor->distance <= 700.0f)
	{
		FVector direction = UKismetMathLibrary::GetDirectionUnitVector(playerLocation, bossActor->GetActorLocation());
		direction.Z = 0.0f;
		playerForcePushLocation = playerLastPos + (-direction * 300.0f);
		FVector tempLocation = FMath::Lerp(playerCharacter->GetActorLocation(), playerForcePushLocation, DeltaSeconds * 4.0f);

		FHitResult HitResult;
		playerCharacter->SetActorLocation(tempLocation, false, &HitResult);

		if (FVector::Distance(playerCharacter->GetActorLocation(), playerForcePushLocation) <= 45.0f) // or if hit wall or minion
		{
			bossActor->isAtk = false;
			bossActor->isNormalPush = false;
		}
	}
}

void AMyAIController::AddMinion(AMinion* minion)
{
	MinionList.Add(minion);
}

void AMyAIController::AddFireMinion(AMinion* minion)
{
	FireMinionList.Add(minion);
}

void AMyAIController::SummonMinion()
{
	for (int i = 0; i < bossActor->bloodPoolList.Num(); i++)
	{
		if (FVector::Distance(bossActor->bloodPoolList[i]->GetActorLocation(), bossActor->GetActorLocation()) <= 700.0f)
		{
			int randNum = FMath::RandRange(0, 1);
			for (int j = 0; j < randNum + 1; j++)
			{
				float x = bloodPoolRadius * cos(FMath::RandRange(0.0f, 360.0f));
				float y = bloodPoolRadius * sin(FMath::RandRange(0.0f, 360.0f));
				FVector spawnLocation = bossActor->bloodPoolList[i]->GetActorLocation() + FVector(x, y, 100.0f);
				auto Minion = GetWorld()->SpawnActor<AMinion>(bossActor->MinionBlueprint->GetOwnerClass(), spawnLocation, FRotator::ZeroRotator);
				AddMinion(Minion);
			}
		}
	}
}

void AMyAIController::ThrowYari()
{
	FVector OutLaunchVelocity;
	FVector targetLocation = playerCharacter->GetActorLocation() - (bossActor->GetActorUpVector() * 20.0f);
	FVector spawnLocation = bossActor->GetActorLocation() + (bossActor->GetActorForwardVector() * 30.0f) + (bossActor->GetActorUpVector() * 90.0f);
	if (UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, spawnLocation, targetLocation, bossActor->yariLaunchSpeed * 0.8f, false, 0.0f, 0, ESuggestProjVelocityTraceOption::DoNotTrace))
	{
		bossActor->AimDirection = OutLaunchVelocity.GetSafeNormal();
	}

	FRotator AimAsRotator = bossActor->AimDirection.Rotation();
	
	if (bossActor->isOnThrone)
	{
		yari->ThrowOnThrone();
	}
	else
	{
		yari->ThrowOnGround(bossActor->yariLaunchSpeed, AimAsRotator);
	}
	
	bossActor->isYariThrow = true;
}

void AMyAIController::CallBackYari()
{
	yari->isBack = true;
}

void AMyAIController::SpitBlood()
{
	bossActor->isAtk = true;
	bossActor->isSpitting = true;
	FVector OutLaunchVelocity;
	FVector targetLocation = playerCharacter->GetActorLocation() - (bossActor->GetActorUpVector() * 20.0f);
	FVector spawnLocation = bossActor->GetActorLocation() + (bossActor->GetActorForwardVector() * 30.0f) + (bossActor->GetActorUpVector() * 90.0f);
	if (UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, spawnLocation, targetLocation, bossActor->bloodLaunchSpeed, false, 0.0f, 0, ESuggestProjVelocityTraceOption::DoNotTrace))
	{
		bossActor->AimDirection = OutLaunchVelocity.GetSafeNormal();
	}

	auto Projectile = GetWorld()->SpawnActor<ABloodProjectile>(bossActor->BloodProjectileBlueprint->GetOwnerClass(), spawnLocation, bossActor->GetActorRotation());
	AActor * projectileActor = Projectile;
	FRotator AimAsRotator = bossActor->AimDirection.Rotation();

	projectileActor->SetActorRotation(AimAsRotator);

	Projectile->LaunchProjectile(bossActor->bloodLaunchSpeed);

	if (drinkBloodDuration > 0)
	{	
		drinkBloodDuration -= 0.5f;
		if(drinkBloodDuration >0) GetWorld()->GetTimerManager().SetTimer(SpitTimer, this, &AMyAIController::SpitBlood, bossActor->spitDelay);		
		else
		{
			drinkBloodDuration = 0.0f;
			bossActor->isAtk = false;
			bossActor->isSpitting = false;
		}
	}
	//delete projectileActor;
}

void AMyAIController::CheckNearestEnemy()
{
	for (int i = 0; i < MinionList.Num(); i++)
	{	
		if (nearestMinionList.Num() < 3 && !nearestMinionList.Contains(MinionList[i]))
		{
			float distance = FVector::Distance(MinionList[i]->GetActorLocation(), playerCharacter->GetActorLocation());
			if (distance <= minionMaxDistance)
			{
				nearestMinionList.Add(MinionList[i]);
				MinionList[i]->enemyType = EnemyType::Melee;
			}
		}	
	}

	//updateDelay += 1;

	//if (updateDelay >= 3)
	//{
		for (int i = 0; i < nearestMinionList.Num(); i++)
		{
			float distance = FVector::Distance(nearestMinionList[i]->GetActorLocation(), playerCharacter->GetActorLocation());
			if (distance > minionMaxDistance)
			{
				nearestMinionList[i]->enemyType = EnemyType::Range;
				nearestMinionList.RemoveAt(i);
			}
		}
	//	updateDelay = 0;
	//}
}

void AMyAIController::DashToPortalOrPlayer(float DeltaSeconds)
{
	if (bossActor->dashCount < 4)
	{
		acceleration = directionMoving * accelerationForce;
		acceleration.Z = 0.0f;

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
		bossActor->SetActorLocation(bossActor->GetActorLocation() + currentVelocity, false, &HitResult);

		LookAtPortal();
	}
	else
	{
		findPlayerTimer += DeltaSeconds;
		if (findPlayerTimer >= findPlayerDuration)
		{
			directionMoving = UKismetMathLibrary::GetDirectionUnitVector(bossActor->GetActorLocation(), playerCharacter->GetActorLocation());
			directionMoving.Z = 0.0f;
			findPlayerTimer = 0.0f;
			playerLastPos = playerCharacter->GetActorLocation();
			acceleration = FVector::ZeroVector;
			currentVelocity = currentVelocity / 3.0f;
		}
		acceleration = directionMoving * accelerationForce;
		acceleration.Z = 0.0f;

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
		bossActor->SetActorLocation(bossActor->GetActorLocation() + currentVelocity, false, &HitResult);
		//bossActor->LookAtPlayer();
		LookAtVelocity();
		if (FVector::Distance(bossActor->GetActorLocation(), playerLastPos) <= 400.0f)
		{
			bossActor->isPortalDash = false;
			bossActor->isAtk = false;
			bossActor->dashCount = 0;
			ResetVelocity();
		}
	}
}

void AMyAIController::FindPortalDirection()
{
	directionMoving = UKismetMathLibrary::GetDirectionUnitVector(bossActor->GetActorLocation(), bossActor->targetPortal->GetActorLocation());
	directionMoving.Z = 0.0f;
}

void AMyAIController::FindPlayerDirection()
{
	directionMoving = UKismetMathLibrary::GetDirectionUnitVector(bossActor->GetActorLocation(), playerCharacter->GetActorLocation());
	directionMoving.Z = 0.0f;
	playerLastPos = playerCharacter->GetActorLocation();
}

void AMyAIController::LookAtVelocity()
{
	FRotator Rot = FRotationMatrix::MakeFromX(currentVelocity).Rotator();
	bossActor->SetActorRotation(Rot);
}

void AMyAIController::LookAtPortal()
{
	FRotator Rot = FRotationMatrix::MakeFromX(directionMoving).Rotator();
	bossActor->SetActorRotation(Rot);
}

void AMyAIController::ResetVelocity()
{
	currentVelocity = FVector::ZeroVector;
	acceleration = FVector::ZeroVector;
}

void AMyAIController::RandomizeRecovery()
{
	recoveryDuration = FMath::RandRange(3.0f, 5.0f);
}

