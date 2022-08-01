﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSGame.h"
#include "TPSPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	state = EEnemyState::IDLE;

	me = Cast<AEnemy>(GetOwner());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (state)
	{
	case EEnemyState::IDLE: 	TickIdle();		break;
	case EEnemyState::MOVE:		TickMove();		break;
	case EEnemyState::ATTACK:	TickAttack();	break;
	case EEnemyState::DAMAGE:	TickDamage();	break;
	case EEnemyState::DIE:		TickDie();		break;
	}

}

void UEnemyFSM::TickIdle()
{
	// 시간이 흐르다가
	currentTime += GetWorld()->GetDeltaSeconds();
	// 현재시간이 idleDelayTime을 초과하면
	if (currentTime > idleDelayTime)
	{
		currentTime = 0;
		// 목적지를 정하고
		/*target = Cast<ATPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()));*/
		target = Cast<ATPSPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
		// 이동상태로 전이하고싶다.
		state = EEnemyState::MOVE;
		PRINT_LOG(TEXT("MOVE"));
	}
}

void UEnemyFSM::TickMove()
{
	// 목적지를 향하는 방향을 만들고 target - me
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// 그 방향으로 이동하고싶다.
	me->AddMovementInput(dir.GetSafeNormal());

	// 목적지와 나의 거리를 구하고
	float dist = dir.Size();
	/*float dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());*/
	// 만약 거리가 attackRange이하라면
	if (dist <= attackRange) {
		// 공격상태로 전이하고싶다.
		state = EEnemyState::ATTACK;
		PRINT_LOG(TEXT("ATTACK"));
	}
}

void UEnemyFSM::TickAttack()
{
	// 공격상태가 되었을때 시간이 흐르다가 
	currentTime += GetWorld()->GetDeltaSeconds();
	// 공격대기시간을 초과하면 공격하고싶다.
	if (currentTime > attackDelayTime)
	{
		// 만약 타겟이 공격사정거리 안에 있지않다면(두 위치의 거리를 구하고, 거리 > attackRange) 
		float dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
		if (dist > attackRange)
		{
			//	이동상태로 전이하고싶다.
			state = EEnemyState::MOVE;
			PRINT_LOG(TEXT("MOVE"));
		}
		else // 그렇지 않다면
		{
			//	공격하고싶다.
			PRINT_LOG(TEXT("ATTACK!!!"));
		}

		currentTime = 0;
	}
}

void UEnemyFSM::TickDamage()
{
}

void UEnemyFSM::TickDie()
{
}

void UEnemyFSM::OnTakeDamage()
{
	// 플레이어가 나를 공격하면 함수를 호출해서 체력을 1 감소시키고싶다.
	hp--;
	// 만약 체력이 0이되면 죽고싶다.
	if (hp <= 0)
	{
		me->Destroy();
	}
}

