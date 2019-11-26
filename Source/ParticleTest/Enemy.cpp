// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "TPSCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	playerPawn = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerActor = Cast<ATPSCharacter>(playerPawn);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::Hi()
{
	TArray<AActor*>AllEnemies;
	TArray<APawn> pawns;
	const TArray<TEnumAsByte<EObjectTypeQuery>> Pawn;
	//UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation(), playerPawn->GetActorLocation(), Pawn, EDrawDebugTrace::ForDuration,nullptr, nullptr,true, nullptr, nullptr, nullptr,0.1f);
}

void AEnemy::SphereLookAt()
{
	ACharacter* playerPawn = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FRotator lookRotationAtPlayer = UKismetMathLibrary::FindLookAtRotation(sphereLooker->K2_GetComponentLocation(), playerPawn->GetActorLocation());
	lookRotationAtPlayer.Roll, lookRotationAtPlayer.Pitch = 0;
	
	sphereLooker->SetWorldRotation(lookRotationAtPlayer);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::SetIsDamaged(bool b)
{
	isDamaged = b;
}

bool AEnemy::GetIsDamaged()
{
	return isDamaged;
}

