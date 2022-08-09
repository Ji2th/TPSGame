// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMoveComponent.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API UPlayerMoveComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()
public:
	UPlayerMoveComponent();


	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PlayerInputBinding(class UInputComponent* PlayerInputComponent) override;

	void OnAxisLookUp(float value);
	void OnAxisTurn(float value);

	// 키를 입력받아서 방향을 만들고 그 방향으로 이동하고싶다.
	// 점프 버튼을 누르면 점프하고싶다.
	FVector dir;
	void OnAxisMoveForward(float value);
	void OnAxisMoveRight(float value);

	void OnActionJump();

	// 왼쪽 쉬프트 버튼을 누르면 달리고싶다. 떼면 걷고싶다.
// 시작할 때 최대속력을 walkSpeed로 하고싶다.
	float walkSpeed = 200;
	float runSpeed = 600;
	void OnActionRunPressed();
	void OnActionRunReleased();
};
