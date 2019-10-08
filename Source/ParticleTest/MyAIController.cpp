// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
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
	maxHealAmount = bossActor->maxHealth * 0.25f;
}

void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	bossActor->distance = FVector::Distance(bossPawn->GetActorLocation(), playerLocation);

	if (!isStart)
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
				bossActor->isAtk = true;
				bossActor->isHealing = true;
				bossActor->healCount += 1;
				UE_LOG(LogTemp, Warning, TEXT("Heal"));
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
			drinkBloodDuration -= DeltaSeconds;
		}

		// Heal Guord
		if (bossActor->isHealing)
		{
			StopMovement();
			currentHealAmount += 3.0f * DeltaSeconds;
			bossActor->currentHealth += 3.0f * DeltaSeconds;

			if (currentHealAmount >= maxHealAmount)
			{
				bossActor->isAtk = false;
				bossActor->isHealing = false;
				currentHealAmount = 0.0f;
			}
		}

		// Drink Blood
		if (bossActor->isDrinking)
		{
			if (drinkBloodDuration < 10.0f) drinkBloodDuration += DeltaSeconds;
			if (drinkBloodDuration >= 10.0f)
			{
				drinkBloodDuration = 10.0f;
				bossActor->isDrinking = false;
				bossActor->isAtk = false;
			}
		}

		// Jump Dash from throne
		if (bossActor->isJumpDash)
		{
			bossActor->JumpDash(DeltaSeconds);
		}
	}
	else // isStart
	{
		yariThrowTimer += DeltaSeconds;
		if (yariThrowTimer >= startYariThrowDuration)
		{
			if (!bossActor->isYariThrow)
			{
				yariThrowTimer = 0.0f;
				isStart = false;
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

void AMyAIController::JumpBackThrone()
{
	bossActor->isAtk = true;
	StopMovement();
	FVector OutLaunchVelocity;
	FVector targetLocation = bossActor->throne->GetComponentLocation();
	FVector startLocation = bossActor->GetActorLocation();
	if (UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, startLocation, targetLocation, bossActor->yariLaunchSpeed * 0.8f, false, 0.0f, 0, ESuggestProjVelocityTraceOption::DoNotTrace))
	{
		bossActor->AimDirection = OutLaunchVelocity.GetSafeNormal();
	}

	FRotator AimAsRotator = bossActor->AimDirection.Rotation();

	bossActor->SetActorRotation(AimAsRotator);
	bossActor->ProjectileMovement->SetVelocityInLocalSpace(OutLaunchVelocity);
	bossActor->ProjectileMovement->Activate();
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
				MinionList.Add(Minion);
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
	
	yari->ThrowOnGround(bossActor->yariLaunchSpeed, AimAsRotator);
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

	if (drinkBloodDuration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(SpitTimer, this, &AMyAIController::SpitBlood, bossActor->spitDelay);
	}
	else
	{
		drinkBloodDuration = 0.0f;
		bossActor->isAtk = false;
		bossActor->isSpitting = false;
	}
}

