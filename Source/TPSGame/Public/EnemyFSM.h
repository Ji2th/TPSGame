// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,
	MOVE,
	ATTACK,
	DAMAGE,
	DIE,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSGAME_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	class AEnemy* me;

	EEnemyState state;

	float currentTime;
	float idleDelayTime = 1;
	float attackRange = 300;
	// 공격상태가 되었을때 시간이 흐르다가 공격대기시간을 초과하면 공격하고싶다.
	UPROPERTY(EditAnywhere)
	float attackDelayTime = 1;
	UPROPERTY(EditAnywhere)
	float damageDelayTime = 1;
	UPROPERTY(EditAnywhere)
	float dieDelayTime = 2;

	class ATPSPlayer* target;
		
	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

	// 플레이어가 호출하는 이벤트 함수다.
	void OnTakeDamage();

	int hp = 3;

	UPROPERTY(EditAnywhere)
	class UEnemyAnim* anim;

	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	// 길 위에 플레이어를 찾지 못했을 때 이동할 목적지
	FVector randLocation;

	// 랜덤 목적지를 생성하는 기능
	// 현재위치, 반경, 성공했다면 담아줄 변수
	bool GetRandomLocationInNavMesh(FVector origin, float radius, FVector& outLoc);

	FRotator rot;
};
