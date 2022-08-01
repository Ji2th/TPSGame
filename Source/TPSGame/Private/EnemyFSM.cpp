// Fill out your copyright notice in the Description page of Project Settings.


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
	// �ð��� �帣�ٰ�
	currentTime += GetWorld()->GetDeltaSeconds();
	// ����ð��� idleDelayTime�� �ʰ��ϸ�
	if (currentTime > idleDelayTime)
	{
		currentTime = 0;
		// �������� ���ϰ�
		/*target = Cast<ATPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()));*/
		target = Cast<ATPSPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
		// �̵����·� �����ϰ�ʹ�.
		state = EEnemyState::MOVE;
		PRINT_LOG(TEXT("MOVE"));
	}
}

void UEnemyFSM::TickMove()
{
	// �������� ���ϴ� ������ ����� target - me
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// �� �������� �̵��ϰ�ʹ�.
	me->AddMovementInput(dir.GetSafeNormal());

	// �������� ���� �Ÿ��� ���ϰ�
	float dist = dir.Size();
	/*float dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());*/
	// ���� �Ÿ��� attackRange���϶��
	if (dist <= attackRange) {
		// ���ݻ��·� �����ϰ�ʹ�.
		state = EEnemyState::ATTACK;
		PRINT_LOG(TEXT("ATTACK"));
	}
}

void UEnemyFSM::TickAttack()
{
	// ���ݻ��°� �Ǿ����� �ð��� �帣�ٰ� 
	currentTime += GetWorld()->GetDeltaSeconds();
	// ���ݴ��ð��� �ʰ��ϸ� �����ϰ�ʹ�.
	if (currentTime > attackDelayTime)
	{
		// ���� Ÿ���� ���ݻ����Ÿ� �ȿ� �����ʴٸ�(�� ��ġ�� �Ÿ��� ���ϰ�, �Ÿ� > attackRange) 
		float dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
		if (dist > attackRange)
		{
			//	�̵����·� �����ϰ�ʹ�.
			state = EEnemyState::MOVE;
			PRINT_LOG(TEXT("MOVE"));
		}
		else // �׷��� �ʴٸ�
		{
			//	�����ϰ�ʹ�.
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
	// �÷��̾ ���� �����ϸ� �Լ��� ȣ���ؼ� ü���� 1 ���ҽ�Ű��ʹ�.
	hp--;
	// ���� ü���� 0�̵Ǹ� �װ�ʹ�.
	if (hp <= 0)
	{
		me->Destroy();
	}
}

