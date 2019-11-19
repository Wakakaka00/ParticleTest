// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
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
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	skeletalMesh = GetMesh();
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	handSocketLocation = GetMesh()->GetSocketLocation("hand_r");
	if(isBreak)
	{
		breakTimer += DeltaTime;
		if (bossPhase == BossPhase::Phase1)
		{
			if (breakTimer >= 4.0f)
			{
				breakTimer = 0.0f;
				isBreak = false;
			}
		}
		else if (bossPhase == BossPhase::Phase2)
		{
			if (breakTimer >= 3.0f)
			{
				breakTimer = 0.0f;
				isBreak = false;
			}
		}
		else if (bossPhase == BossPhase::Phase3)
		{
			if (breakTimer >= 2.0f)
			{
				breakTimer = 0.0f;
				isBreak = false;
			}
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
	FRotator Rot = FRotationMatrix::MakeFromX(playerPawn->GetActorLocation() - GetActorLocation()).Rotator();
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
	if(!isBreak) isBreak = true;
}



