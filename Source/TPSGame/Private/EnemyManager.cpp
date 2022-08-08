// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(timerMakeEnemy, this, &AEnemyManager::MakeEnemy, FMath::RandRange(min, max), false);
}


void AEnemyManager::MakeEnemy()
{
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorTransform());


	GetWorldTimerManager().SetTimer(timerMakeEnemy, this, &AEnemyManager::MakeEnemy, FMath::RandRange(min, max), false);
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

