// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPSGAME_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 랜덤한 시간에 적을 생성하고싶다.
	// 적의 위치는 SpawnList의 임의의 한 곳으로 정하고싶다.
	UPROPERTY(EditAnywhere)
	float min;
	UPROPERTY(EditAnywhere)
	float max;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> enemyFactory;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> spawnList;

	FTimerHandle timerMakeEnemy;

	void MakeEnemy();

};
