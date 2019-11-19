// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"


// Sets default values
ATPSCharacter::ATPSCharacter()
{
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Default Number Set Ups
	
	///Maximum Distance Which Player Can Target Lock On Enemies
	MaximumDistance = 13008.3603516f;

	//Maximum Health
	MaximumHealth = 100.0f;
	MinimumHealth = 0.0f;
	CurrentHealth = 100.0f;
	AttackCounts = 0;


	enemyTag = TEXT("Enemy");

}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	
}
// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	if (isPushingBack)
	{
		pushBackTimer += DeltaTime;
		if (pushBackTimer >= 1.5f)
		{
			isPushingBack = false;
			pushBackTimer = 0.0f;
		}
	}
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindInputs();
}



//Lock on functions


void ATPSCharacter::SwitchToBoss_Implementation()
{
	if (TargetLocked)
	{
		TargetLocked = false;
		FOutputDeviceNull ar;
		NearestTarget->CallFunctionByNameWithArguments(TEXT("LockOnFlip"), ar, NULL, true);
		UKismetSystemLibrary::K2_PauseTimer(this, TEXT("ToggleLockOn"));
		NearestTarget = NULL;
	}
	else
	{
		EnemyElement = bossActor;
		LockOnDone();
		SetLockOnToTarget();
		
	}
}

void ATPSCharacter::LockOnFunction_Implementation(bool OnlyBoss)
{
	if (TargetLocked)
	{
		TargetLocked = false;
		FOutputDeviceNull ar;
		NearestTarget->CallFunctionByNameWithArguments(TEXT("LockOnFlip"),ar,NULL,true);
		UKismetSystemLibrary::K2_PauseTimer(this, TEXT("ToggleLockOn"));
		NearestTarget = NULL;
		
		
	
	}
	else
	{
		EnemyElement = bossActor;
		LockOnDone();
		SetLockOnToTarget();
		
	}

}

void ATPSCharacter::LockOnDone_Implementation()
{
	NearestTarget = EnemyElement;
	ClosestTargetDistance = NearestTarget->PlayerToEnemyDistance;
	
	
	
}
void ATPSCharacter::SetLockOnToTarget_Implementation()
{
	
	if (ClosestTargetDistance < MaximumDistance)
	{
		FOutputDeviceNull ar;
		NearestTarget->CallFunctionByNameWithArguments(TEXT("LockOnFlip"), ar, NULL, true);
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("ToggleLockOn"), GetWorld()->GetDeltaSeconds(), true);
		TargetLocked = true;
	}
}

void ATPSCharacter::SwitchLeft_Implementation()
{
	if (TargetLocked)
	{
		FoundNewTarget = false;
		bool MaybeSwitchDown = false;
		bool MaybeSwitchUp = false;
		ClosestTargetRotation = 340.0f;
		ClosestTargetDistance = MaximumDistance;
		ClosestTargetDistanceToTargetedEnemy = MaximumDistance;
		//Get all enemies in the scene
		TArray<AActor*> FoundActors;
		
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), enemyTag, FoundActors);
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			if (FoundActors[i] != NearestTarget)
			{
				AEnemy* enemy = Cast<AEnemy>(FoundActors[i]);
				if (enemy->CanBeTargeted)
				{
					EnemyElement = enemy;
					FRotator lookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(NearestTarget->sphereLooker->GetComponentLocation(), EnemyElement->GetActorLocation());
					bool selectBool = lookAtEnemyRotation.Yaw < 0.0f;
					float selectFloat1 = UKismetMathLibrary::SelectFloat(lookAtEnemyRotation.Yaw + 360.0f, lookAtEnemyRotation.Yaw, selectBool);


					FRotator lookAtNearestEnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetGlobalPlayer()->GetActorLocation(), NearestTarget->sphereLooker->GetComponentLocation());

					bool selectBool2 = lookAtNearestEnemyRotation.Yaw < 0.0f;
					float selectFloat2 = UKismetMathLibrary::SelectFloat(lookAtNearestEnemyRotation.Yaw + 360.0f, lookAtNearestEnemyRotation.Yaw, selectBool2);

					float outComeFloat = selectFloat1 - selectFloat2;

					bool selectBool3 = outComeFloat < 0.0f;

					float selectFloat3 = UKismetMathLibrary::SelectFloat(outComeFloat + 360.0f, outComeFloat, selectBool3);

					///////////////////////	
					bool isInRange = UKismetMathLibrary::InRange_FloatFloat(selectFloat3, 210.0f, 350.0f);

					if (!MaybeSwitchDown)MaybeSwitchDown = UKismetMathLibrary::InRange_FloatFloat(selectFloat3, 180.0f, 210.0f);
					if (!MaybeSwitchUp)MaybeSwitchUp = UKismetMathLibrary::InRange_FloatFloat(selectFloat3, 350.0f, 360.0f);
					float distanceBetweenEnemy = (EnemyElement->GetActorLocation() - NearestTarget->GetActorLocation()).Size();
					if (isInRange)
					{
						if (distanceBetweenEnemy < ClosestTargetDistanceToTargetedEnemy)
						{
							ClosestTargetDistanceToTargetedEnemy = distanceBetweenEnemy;						
							FoundNewTarget = true;
							NewTarget = EnemyElement;
						}
					}
					
				}
			}

		}
		if (FoundNewTarget)
		{

			FOutputDeviceNull ar;
			NearestTarget->CallFunctionByNameWithArguments(TEXT("LockOnFlip"), ar, NULL, true);
			SwitchDone();
			SetLockOnToTarget();
		}
		else
		{
			if (MaybeSwitchDown)
			{
				SwitchDown();

			}
			else if (MaybeSwitchUp)
			{
				SwitchUp();
			}
		}



	}
	
	
}



void ATPSCharacter::SwitchUp_Implementation()
{
	if (TargetLocked)
	{
		
		ClosestTargetRotation = 360.0f;
		ClosestTargetDistance = MaximumDistance;
		ClosestTargetDistanceToTargetedEnemy = MaximumDistance;
		//Get all enemies in the scene
		TArray<AActor*> FoundActors;
		
		FName enemyTag = TEXT("Enemy");
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), enemyTag, FoundActors);
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			if (FoundActors[i] != NearestTarget)
			{
				AEnemy* enemy = Cast<AEnemy>(FoundActors[i]);
				if (enemy->CanBeTargeted)
				{
					EnemyElement = enemy;

					FRotator lookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(NearestTarget->sphereLooker->GetComponentLocation(), EnemyElement->GetActorLocation());
					bool selectBool = lookAtEnemyRotation.Yaw < 0.0f;
					float selectFloat1 = UKismetMathLibrary::SelectFloat(lookAtEnemyRotation.Yaw + 360.0f, lookAtEnemyRotation.Yaw, selectBool);


					FRotator lookAtNearestEnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetGlobalPlayer()->GetActorLocation(), NearestTarget->sphereLooker->GetComponentLocation());

					bool selectBool2 = lookAtNearestEnemyRotation.Yaw < 0.0f;
					float selectFloat2 = UKismetMathLibrary::SelectFloat(lookAtNearestEnemyRotation.Yaw + 360.0f, lookAtNearestEnemyRotation.Yaw, selectBool2);

					float outComeFloat = selectFloat1 - selectFloat2;

					bool selectBool3 = outComeFloat < 0.0f;

					float selectFloat3 = UKismetMathLibrary::SelectFloat(outComeFloat + 360.0f, outComeFloat, selectBool3);

					
					bool isInRange = false;

					

					///////////////////////
					


					float distanceBetweenEnemy = (EnemyElement->GetActorLocation() - NearestTarget->GetActorLocation()).Size();
					

					if (selectFloat3 > 335.0f && selectFloat3<360.0f || selectFloat3>0.0f && selectFloat3 < 45.0f)
					{
						isInRange = true;
					}
					if (isInRange)
					{					
							if (distanceBetweenEnemy < ClosestTargetDistanceToTargetedEnemy)
							{
								ClosestTargetDistanceToTargetedEnemy = distanceBetweenEnemy;
								FoundNewTarget = true;				
								NewTarget = EnemyElement;
							}

						}
					
				}
			}

		}
		if (FoundNewTarget)
		{

			FOutputDeviceNull ar;
			NearestTarget->CallFunctionByNameWithArguments(TEXT("LockOnFlip"), ar, NULL, true);
			SwitchDone();
			SetLockOnToTarget();
		}
		else
		{
			
		}



	}
}

void ATPSCharacter::SwitchDown_Implementation()
{
	if (TargetLocked)
	{		
		ClosestTargetRotation = 0.0f;
		ClosestTargetDistance = MaximumDistance;
		ClosestTargetDistanceToTargetedEnemy = MaximumDistance;
		TArray<AActor*> FoundActors;
		FName enemyTag = TEXT("Enemy");
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), enemyTag, FoundActors);
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			if (FoundActors[i] != NearestTarget)
			{
				AEnemy* enemy = Cast<AEnemy>(FoundActors[i]);
				if (enemy->CanBeTargeted)
				{
					EnemyElement = enemy;
					
					FRotator lookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(NearestTarget->sphereLooker->GetComponentLocation(), EnemyElement->GetActorLocation());
					bool selectBool = lookAtEnemyRotation.Yaw < 0.0f;
					float selectFloat1 = UKismetMathLibrary::SelectFloat(lookAtEnemyRotation.Yaw + 360.0f, lookAtEnemyRotation.Yaw, selectBool);


					FRotator lookAtNearestEnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetGlobalPlayer()->GetActorLocation(), NearestTarget->sphereLooker->GetComponentLocation());

					bool selectBool2 = lookAtNearestEnemyRotation.Yaw < 0.0f;
					float selectFloat2 = UKismetMathLibrary::SelectFloat(lookAtNearestEnemyRotation.Yaw + 360.0f, lookAtNearestEnemyRotation.Yaw, selectBool2);

					float outComeFloat = selectFloat1 - selectFloat2;

					bool selectBool3 = outComeFloat < 0.0f;

					float selectFloat3 = UKismetMathLibrary::SelectFloat(outComeFloat + 360.0f, outComeFloat, selectBool3);

					bool isInRange = false;

					float distanceBetweenEnemy = (EnemyElement->GetActorLocation() - NearestTarget->GetActorLocation()).Size();
					
					if (selectFloat3 > 130.0f && selectFloat3<180.0f || selectFloat3>180.0f && selectFloat3 < 245.0f)
					{
						isInRange = true;
					}
					if (isInRange)
					{
						if (distanceBetweenEnemy < ClosestTargetDistanceToTargetedEnemy)
						{
							ClosestTargetDistanceToTargetedEnemy = distanceBetweenEnemy;
							FoundNewTarget = true;
							NewTarget = EnemyElement;
						}

					}

				}
			}

		}
		if (FoundNewTarget)
		{

			FOutputDeviceNull ar;
			NearestTarget->CallFunctionByNameWithArguments(TEXT("LockOnFlip"), ar, NULL, true);
			SwitchDone();
			SetLockOnToTarget();
		}
		else
		{

		}



	}
}



float ATPSCharacter::AngleDir(FVector fwd, FVector targetDir, FVector up)
{
	FVector perp = FVector::CrossProduct(fwd, targetDir);
	float dir = FVector::DotProduct(perp, up);

	if (dir > 0.0f)
	{
		return 1.0f;
	}
	else if (dir < 0.0f)
	{
		return -1.0f;
	}
	else
	{
		return 0.0f;
	}
	
	
}


void ATPSCharacter::SwitchRight_Implementation()
{
	if (TargetLocked)
	{
		FoundNewTarget = false;
		ClosestTargetDistance = MaximumDistance;
		//Get all enemies in the scene
		TArray<AActor*> FoundActors;
		FName enemyTag = TEXT("Enemy");
		ClosestTargetDistanceToTargetedEnemy = MaximumDistance;
		bool MaybeSwitchDown = false;
		bool MaybeSwitchUp = false;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), enemyTag, FoundActors);
		for (int i = 0; i < FoundActors.Num(); i++)
		{
			if (FoundActors[i] != NearestTarget)
			{
				AEnemy* enemy = Cast<AEnemy>(FoundActors[i]);
				if (enemy->CanBeTargeted)
				{
					EnemyElement = enemy;


					FRotator lookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(NearestTarget->sphereLooker->GetComponentLocation(), EnemyElement->GetActorLocation());
					bool selectBool = lookAtEnemyRotation.Yaw < 0.0f;
					float selectFloat1 = UKismetMathLibrary::SelectFloat(lookAtEnemyRotation.Yaw + 360.0f, lookAtEnemyRotation.Yaw, selectBool);


					FRotator lookAtNearestEnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetGlobalPlayer()->GetActorLocation(), NearestTarget->sphereLooker->GetComponentLocation());

					bool selectBool2 = lookAtNearestEnemyRotation.Yaw < 0.0f;
					float selectFloat2 = UKismetMathLibrary::SelectFloat(lookAtNearestEnemyRotation.Yaw + 360.0f, lookAtNearestEnemyRotation.Yaw, selectBool2);

					float outComeFloat = selectFloat1 - selectFloat2;

					bool selectBool3 = outComeFloat < 0.0f;

					float selectFloat3 = UKismetMathLibrary::SelectFloat(outComeFloat + 360.0f, outComeFloat, selectBool3);
					//0.0    180
					bool isInRange = UKismetMathLibrary::InRange_FloatFloat(selectFloat3, 30.0f, 145.0f);			
					float distanceBetweenEnemy = (EnemyElement->GetActorLocation() - NearestTarget->GetActorLocation()).Size();
					if(!MaybeSwitchDown)MaybeSwitchDown = UKismetMathLibrary::InRange_FloatFloat(selectFloat3, 145.0f ,180.0f);
					if (!MaybeSwitchUp)MaybeSwitchUp = UKismetMathLibrary::InRange_FloatFloat(selectFloat3, 0.0f, 30.0f);
					FString TheFloatStr = FString::SanitizeFloat(selectFloat3);
								

					/*if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), MaybeSwitchDown ? TEXT("true") : TEXT("false")));
					}*/
						if (isInRange)
						{
							if (distanceBetweenEnemy < ClosestTargetDistanceToTargetedEnemy)
							{
								ClosestTargetDistanceToTargetedEnemy = distanceBetweenEnemy;				
								FoundNewTarget = true;
								NewTarget = EnemyElement;
							}
						}			
					
				}
			}
		


		}

		if (FoundNewTarget)
		{

			FOutputDeviceNull ar;
			NearestTarget->CallFunctionByNameWithArguments(TEXT("LockOnFlip"), ar, NULL, true);
			SwitchDone();
			SetLockOnToTarget();
		}
		else 
		{
			
			if (MaybeSwitchDown)
			{
				SwitchDown();
				
			}
			else if (MaybeSwitchUp)
			{
				SwitchUp();
			}	
			
		}
	}



}


void ATPSCharacter::SwitchDone_Implementation()
{
	NearestTarget = NewTarget;
	ClosestTargetDistance = NearestTarget->PlayerToEnemyDistance;
	
}



//Melee Combat

void ATPSCharacter::DoAttacks(bool DoCharge)
{
	

	switch (SwitchMode)
	{
		case ESwitchModeEnum::ES_Light:
		{
			if (!DoCharge)
			{
				if (AttackCounts == 0)
				{
					PlayAnimMontage(montages[0], 0.5f);
					
					AttackCounts = 1;
				}
				else if (AttackCounts == 1)
				{
					PlayAnimMontage(montages[1], 0.8f);
					AttackCounts = 2;
				}
				else if (AttackCounts == 2)
				{
					PlayAnimMontage(montages[2], 0.6f);
					AttackCounts = 0;
				}
				
				
				

				
			}
			else if (DoCharge)
			{
				PlayAnimMontage(montages[3], 0.3f);
				
			}
			break;
		}
		case ESwitchModeEnum::ES_Heavy:
		{
			if (AttackCounts == 0)
			{

				PlayAnimMontage(montages[0], 0.7f);
				AttackCounts = 1;
			}
			else if (AttackCounts == 1)
			{
				PlayAnimMontage(montages[1], 0.7f);
				AttackCounts = 2;
			}
			else if (AttackCounts == 2)
			{
				PlayAnimMontage(montages[2], 0.7f);
				AttackCounts = 0;
			}
			
			 break;
		}

	}
}

void ATPSCharacter::ResetCombo_Implementation()
{
	CanContinueCombo = false;
	AttackCounts = 0;
	inAttackAnimation = false;
	PostCanContinueCombo = false;
	HeavyChargeAttackCounterOrder = 0;
	CanAttack = true;
}

void ATPSCharacter::ContinueCombo()
{
	
	
	if (CanContinueCombo &&!ReservedChargedAttack)//Player clicked before post attack and it continue combo
	{
		CanContinueCombo = false;
		DoAttacks(false);
		
		
	}
	
	else if (!CanContinueCombo&&ReservedChargedAttack)// player hold before attack finish
	{
		if (HeavyChargeAttackCounterOrder == AttackCounts)
		{
			ReservedChargedAttack = false;
			HeavyChargeAttackCounterOrder = 0;
			DoAttacks(true);
			;
		}
		
	}
	else if (CanContinueCombo&&ReservedChargedAttack)// player hold before attack finish
	{
		
		if (HeavyChargeAttackCounterOrder == AttackCounts)
		{
			CanContinueCombo = false;
			ReservedChargedAttack = false;
			HeavyChargeAttackCounterOrder = 0;
			DoAttacks(true);
			
		}
		else
		{
			CanContinueCombo = false;
			DoAttacks(false);
			
		}
		
	}
	else if (!CanContinueCombo && !ReservedChargedAttack)// Player didnt hold neither click so it give another oppotunity
	{
		
		PostCanContinueCombo = true;
		CanContinueCombo = false;
		
	}
	

	// NEXT

	
}

void ATPSCharacter::SetCanContinueCombo(bool b)
{
	CanContinueCombo = b;
}
void ATPSCharacter::Dash()
{

	if (TargetLocked)
	{
		FVector launchDir = (NearestTarget->GetActorLocation() - GetGlobalPlayer()->GetActorLocation()) *20.0f;
		launchDir.Z = 0;
		GetCharacterMovement()->Launch(launchDir);
		if (FVector::Distance(NearestTarget->GetActorLocation(), GetGlobalPlayer()->GetActorLocation()) < 150.0f)
		{
			UKismetSystemLibrary::K2_PauseTimer(this, TEXT("Dash"));
			PlayerAttackedLight();
		}
		
	}
}

void ATPSCharacter::PlayerAttackedLight()
{
	if (CanAttack == false)return;
	
	else if (inAttackAnimation && !PostCanContinueCombo && !CanContinueCombo) // Second attack before postcancontinueattack
	{
		
		CanContinueCombo = true;
		
		
	}
	 else if(!inAttackAnimation && !CanContinueCombo) // First attack
	{
		inAttackAnimation = true;	
		DoAttacks(false);
		
	}
	 else if (inAttackAnimation && PostCanContinueCombo && !CanContinueCombo)//Last phase after can combo event for any last chance
	{
	
		DoAttacks(false);
		PostCanContinueCombo = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Press attack in Post"));
		
	}
	
	

}



void ATPSCharacter::PushBack(float force, FVector pushLocation)
{
	isPushingBack = true;
	FVector direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), pushLocation);
	direction.Z = 0.0f;
	//FVector playerForcePushLocation = lastPos + (-direction * pushBackDistance);
	LaunchCharacter(-direction * force, true, true);
}

ACharacter * ATPSCharacter::GetGlobalPlayer()
{
	
	 ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	 return player;
	
	
	

}

void ATPSCharacter::Movement(FVector direction,float v)
{
	
	
	GetGlobalPlayer()->AddMovementInput(direction, v);
	
	
	
}

//Inputs
void ATPSCharacter::MoveForward(float value)
{
	
	if (CanMove)
	{
		if (!ShouldRotateLockOn)
		{
			FRotator playerRotation = GetGlobalPlayer()->GetControlRotation();
			FRotator desiredRotation = FRotator(0.0f, playerRotation.Yaw, 0.0f);
			FVector desiredDir = FRotationMatrix(desiredRotation).GetScaledAxis(EAxis::X);
			targetForwardVector = desiredDir;
		}
		if (TargetLocked )
		{
			Movement(targetForwardVector, value);
		}
		else 
		{
			FRotator playerRotation = GetGlobalPlayer()->GetControlRotation();
			FRotator desiredRotation = FRotator(0.0f, playerRotation.Yaw, 0.0f);
			FVector desiredDir = FRotationMatrix(desiredRotation).GetScaledAxis(EAxis::X);
			Movement(desiredDir, value);
		}		
	}
}
void ATPSCharacter::MoveRight(float value)
{
	if (CanMove)
	{
		if (!ShouldRotateLockOn)
		{
			FRotator playerRotation = GetGlobalPlayer()->GetControlRotation();
			FRotator desiredRotation = FRotator(0.0f, playerRotation.Yaw, 0.0f);
			FVector desiredDir = FRotationMatrix(desiredRotation).GetScaledAxis(EAxis::Y);
			targetRightVector = desiredDir;
		}
		if (TargetLocked)
		{
			Movement(targetRightVector, value);
		}
		else 
		{
			FRotator playerRotation = GetGlobalPlayer()->GetControlRotation();
			FRotator desiredRotation = FRotator(0.0f, playerRotation.Yaw, 0.0f);
			desiredDir = FRotationMatrix(desiredRotation).GetScaledAxis(EAxis::Y);
			Movement(desiredDir, value);
		}
	
	}
		
}

//Get Player Actor

void ATPSCharacter::BindInputs()
{
		InputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);
	
	
}


