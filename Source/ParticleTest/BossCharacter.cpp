// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TPSCharacter.h"
#include "MyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	PushCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	PushCapsule->InitCapsuleSize(83.0f, 30.0f);;
	PushCapsule->SetCollisionProfileName(TEXT("Trigger"));
	PushCapsule->SetupAttachment(RootComponent);
	PushCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->bAutoActivate = false;

	maxHealth = 1000.0f;
	currentHealth = maxHealth;
	bossPhase = BossPhase::Phase1;
	bossState = BossState::Recovery;
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	skeletalMesh = GetMesh();
	aiController = Cast<AMyAIController>(GetController());
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(handSocketLocation == FVector::ZeroVector) handSocketLocation = GetMesh()->GetSocketLocation("Bip001-R-Hand");
	if(bossState == BossState::Break)
	{
		breakTimer += DeltaTime;
		if (bossPhase == BossPhase::Phase1)
		{
			if (breakTimer >= 4.0f)
			{
				breakTimer = 0.0f;
				bossState = BossState::Recovery;
			}
		}
		else if (bossPhase == BossPhase::Phase2)
		{
			if (breakTimer >= 3.0f)
			{
				breakTimer = 0.0f;
				bossState = BossState::Recovery;
			}
		}
		else if (bossPhase == BossPhase::Phase3)
		{
			if (breakTimer >= 2.0f)
			{
				breakTimer = 0.0f;
				bossState = BossState::Recovery;
			}
		}
	}
	else if (bossState == BossState::Vulnerable)
	{
		vulnerableTimer += DeltaTime;
		if (vulnerableTimer >= vulnerableDuration)
		{
			LookAtPlayer();
			vulnerableTimer = 0.0f;
			float distance = FVector::Distance(GetActorLocation(), playerActor->GetActorLocation());
			if(distance <= 700.0f) playerActor->PushBack(pushBackForce, GetActorLocation());		
			bossState = BossState::Recovery;
			isAtk = true;
			JumpBackThrone();
		}
	}
}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABossCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		isNormalPush = true;
		isAtk = true;
		playerLastPos = playerPawn->GetActorLocation();
		SetPushCollison(false);
	}
}

void ABossCharacter::SetPushCollison(bool b)
{
	if(!b) PushCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	else PushCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABossCharacter::LookAtPlayer()
{
	FVector playerPos = playerPawn->GetActorLocation();
	FVector bossPos = GetActorLocation();
	playerPos.Z = 0.0f;
	bossPos.Z = 0.0f;
	FRotator Rot = FRotationMatrix::MakeFromX(playerPos - bossPos).Rotator();
	SetActorRotation(Rot);
}

void ABossCharacter::Attack()
{

}

void ABossCharacter::FindNearestPortal()
{	
	if (dashCount == 0)
	{
		targetPortal = portalList[0];
	}
	else
	{
		for (int i = 0; i < portalList.Num(); i++)
		{
			if (isLeft)
			{
				if (!portalList[i]->isLeft)
				{
					targetPortal = portalList[i];
					break;
				}
			}
			else
			{
				if (portalList[i]->isLeft)
				{
					targetPortal = portalList[i];
					break;
				}
			}
		}
	}
	for (int i = 0; i < portalList.Num(); i++)
	{
		if (portalList[i] != targetPortal)
		{
			if (dashCount == 0)
			{
				float shortestDistance = FVector::Distance(targetPortal->GetActorLocation(), hitList[0].ImpactPoint);
				float distance = FVector::Distance(portalList[i]->GetActorLocation(), hitList[0].ImpactPoint);
				if (distance < shortestDistance)
				{
					targetPortal = portalList[i];
				}
			}
			else
			{
				if (isLeft)
				{
					if (!portalList[i]->isLeft)
					{
						float shortestDistance = FVector::Distance(targetPortal->GetActorLocation(), hitList[1].ImpactPoint);
						float distance = FVector::Distance(portalList[i]->GetActorLocation(), hitList[1].ImpactPoint);
						if (distance < shortestDistance)
						{
							targetPortal = portalList[i];
						}
					}
				}
				else
				{
					if (portalList[i]->isLeft)
					{
						float shortestDistance = FVector::Distance(targetPortal->GetActorLocation(), hitList[1].ImpactPoint);
						float distance = FVector::Distance(portalList[i]->GetActorLocation(), hitList[1].ImpactPoint);
						if (distance < shortestDistance)
						{
							targetPortal = portalList[i];
						}
					}
				}
			}
		}
	}

	isLeft = targetPortal->isLeft;
}

void ABossCharacter::Break()
{
	bossState = BossState::Break;
}

void ABossCharacter::SetVulnerable()
{
	bossState = BossState::Vulnerable;
	PlayVulnerable();
}

bool ABossCharacter::isAttackingVital()
{
	FVector currentPos = GetActorLocation();
	currentPos.Z = 0.0f;
	FVector playerPos = playerPawn->GetActorLocation();
	playerPos.Z = 0.0f;
	FVector direction = UKismetMathLibrary::GetDirectionUnitVector(currentPos, playerPos);
	float dot = FVector::DotProduct(direction, GetActorForwardVector());
	float angle = FMath::RadiansToDegrees(FMath::Acos(dot));
	if (angle >= 150.0f)
	{
		return true;
	}
	return false;
}

void ABossCharacter::ResetAttack()
{
	bossState = BossState::Recovery;
	isAtk = false;
}

void ABossCharacter::TrackingPlayer()
{
	isTracking = true;
}

void ABossCharacter::StopTrackingPlayer()
{
	isTracking = false;
	aiController->StopMovement();
}

bool ABossCharacter::GetIsTracking()
{
	return isTracking;
}



