// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodProjectile.h"

// Sets default values
ABloodProjectile::ABloodProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->bAutoActivate = false;
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(20.0f, 20.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABloodProjectile::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ABloodProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABloodProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABloodProjectile::LaunchProjectile(float speed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * speed);
	ProjectileMovement->Activate();
}

void ABloodProjectile::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag("Floor"))
		{
			auto holder = GetWorld()->SpawnActor<AActor>(BloodPoolBlueprint, SweepResult.ImpactPoint, FRotator::ZeroRotator);

			Destroy();
		}
	}
}

