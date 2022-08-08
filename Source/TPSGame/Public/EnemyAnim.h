// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState animState;

	// 애니메이션에서 공격대기에서 공격으로 전환하는 조건
	// Edit / Visible
	// Anywhere / DefaultOnly / InstanceOnly
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttackPlay;

	UFUNCTION(BlueprintCallable)
	void OnAttackFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDamageAnimation(FName sectionName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDiePlay;
};
