// Fill out your copyright notice in the Description page of Project Settings.


#include "Yari.h"
#include "Engine/World.h"
#include "MyAIController.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "BloodPool.h"
#include "TimerManager.h"

// Sets default values
AYari::AYari()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->bAutoActivate = false;

	//SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	//RootComponent = SceneRoot;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->InitBoxExtent(FVector(15.0f, 15.0f, 15.0f));
	TriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
	TriggerBox->AttachTo(RootComponent);
	TriggerBox->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AYari::OnOverlapBegin);

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(5.0f, 10.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("NoCollision"));
	TriggerCapsule->AttachTo(RootComponent);
	TriggerCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AYari::OnOverlapPlayer);
}


// Called when the game starts or when spawned
void AYari::BeginPlay()
{
	Super::BeginPlay();
	bossActor = Cast<ABossCharacter>(GetParentActor());
	bossAI = Cast<AMyAIController>(bossActor->GetController());
	playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerActor = Cast<ATPSCharacter>(playerCharacter);
	gravityScale = 1.5f;
	initLocation = FVector(-3.89f, -10.28f, 24.6f);
	initRotation = FRotator(49.0f, -74.7f, -168.3f);
}

// Called every frame
void AYari::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isThrowing && !bossActor->isOnThrone)
	{
		FRotator rot = FRotationMatrix::MakeFromX(GetVelocity()).Rotator();
		SetActorRotation(rot);
	}

	if(isBack)
	{
		BackToHandSocket(DeltaTime);
	}
}

void AYari::ThrowOnGround(float speed,FRotator angle)
{
	//GetStaticMeshComponent()->SetSimulatePhysics(true);
	isThrowing = true;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(angle);	
	ProjectileMovement->ProjectileGravityScale = gravityScale;
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * speed); 
	ProjectileMovement->Activate();
}

void AYari::StopYari()
{	
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	//ProjectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
	ProjectileMovement->StopMovementImmediately();
	GetStaticMeshComponent()->SetSimulatePhysics(false);
	isThrowing = false;
	bossActor->CallBackYari();
	TriggerCapsule->SetCollisionProfileName("NoCollision");
}


void AYari::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Box Collider
	if (OtherActor->ActorHasTag("Floor"))
	{	
		UE_LOG(LogTemp, Warning, TEXT("Floor"));
		if (isFire)
		{
			for (int i = 0; i < bossActor->bloodPoolList.Num(); i++)
			{
				if (FVector::Distance(TriggerBox->GetComponentLocation(), bossActor->bloodPoolList[i]->GetActorLocation()) <= 700.0f)
				{
					auto Minion = GetWorld()->SpawnActor<AMinion>(bossActor->MinionBlueprint->GetOwnerClass(), bossActor->bloodPoolList[i]->GetActorLocation(), FRotator::ZeroRotator);
					ABloodPool* bp = Cast<ABloodPool>(bossActor->bloodPoolList[i]);
					bp->destroySpeed = 2.0f;
					Minion->Initialize(isFire);
					bossAI->AddFireMinion(Minion);
					//delete bp;
				}
			}
		}		
		//GetWorld()->GetTimerManager().SetTimer(StopTimer, this, &AYari::StopYari, 0.03f);
		StopYari();
		// AOE when hit ground
		if (!isDamaged)
		{
			if (FVector::Distance(TriggerBox->GetComponentLocation(), playerCharacter->GetActorLocation()) <= 600.0f)
			{
				if (!playerActor->isPushingBack)
				{
					playerActor->PushBack(bossActor->pushBackForce, TriggerBox->GetComponentLocation());
					DamagePlayer();
				}
			}
		}		
	}
}

void AYari::OnOverlapPlayer(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Capsule collider
	if (OtherComp->ComponentHasTag("Player"))
	{
		isDamaged = true;
		playerActor->PushBack(bossActor->pushBackForce, GetActorLocation());
		DamagePlayer();
	}
}

void AYari::BackToHandSocket(float DeltaSeconds)
{
	TriggerCapsule->SetCollisionProfileName("Trigger");
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), bossActor->handSocketLocation, DeltaSeconds, 3.0f));
	float distance = FVector::Distance(GetActorLocation(), bossActor->handSocketLocation);
	if (distance <= 30.0f)
	{
		isDamaged = false;
		TriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
		TriggerCapsule->SetCollisionProfileName("NoCollision");
		FName handSocketName = TEXT("hand_r");
		K2_AttachRootComponentTo(bossActor->skeletalMesh, handSocketName, EAttachLocation::SnapToTarget,true);
		isBack = false;
		SetActorRelativeLocation(initLocation, false, nullptr, ETeleportType::None);
		SetActorRelativeRotation(initRotation, false, nullptr, ETeleportType::None);
		bossActor->isYariThrow = false;
	}
}

