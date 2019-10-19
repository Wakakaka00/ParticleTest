// Fill out your copyright notice in the Description page of Project Settings.


#include "Yari.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
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
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->AttachTo(RootComponent);
	TriggerBox->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AYari::OnOverlapBegin);
}


// Called when the game starts or when spawned
void AYari::BeginPlay()
{
	Super::BeginPlay();
	bossActor = Cast<ABossCharacter>(GetParentActor());
	gravityScale = 1.5f;
	initLocation = FVector(-3.89f, -10.28f, 24.6f);
	initRotation = FRotator(49.0f, -74.7f, -168.3f);
}

// Called every frame
void AYari::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isThrowing)
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
}


void AYari::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Floor"))
	{	
		//GetWorld()->GetTimerManager().SetTimer(StopTimer, this, &AYari::StopYari, 0.03f);
		StopYari();
		UE_LOG(LogTemp, Warning, TEXT("Stop"));
	}
}

void AYari::BackToHandSocket(float DeltaSeconds)
{
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), bossActor->handSocketLocation, DeltaSeconds, 3.0f));
	float distance = FVector::Distance(GetActorLocation(), bossActor->handSocketLocation);
	if (distance <= 30.0f)
	{
		FName handSocketName = TEXT("hand_r");
		K2_AttachRootComponentTo(bossActor->skeletalMesh, handSocketName, EAttachLocation::SnapToTarget,true);
		isBack = false;
		SetActorRelativeLocation(initLocation, false, nullptr, ETeleportType::None);
		SetActorRelativeRotation(initRotation, false, nullptr, ETeleportType::None);
		bossActor->isYariThrow = false;
	}
}

