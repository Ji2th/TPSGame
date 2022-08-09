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

	// 스폰목록에 있는 항목중 하나를 랜덤으로 선택하고싶다.
	int randIndex = FMath::RandRange(0, spawnList.Num() - 1);
	auto actor = spawnList[randIndex];
	// 그 액터의 Transform을 사용하고싶다.

	GetWorld()->SpawnActor<AEnemy>(enemyFactory, actor->GetActorTransform());


	GetWorldTimerManager().SetTimer(timerMakeEnemy, this, &AEnemyManager::MakeEnemy, FMath::RandRange(min, max), false);
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

