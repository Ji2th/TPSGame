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
	// ���ݻ��°� �Ǿ����� �ð��� �帣�ٰ� ���ݴ��ð��� �ʰ��ϸ� �����ϰ�ʹ�.
	float attackDelayTime = 1;
	class ATPSPlayer* target;
		
	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

	// �÷��̾ ȣ���ϴ� �̺�Ʈ �Լ���.
	void OnTakeDamage();

	int hp = 3;

};
