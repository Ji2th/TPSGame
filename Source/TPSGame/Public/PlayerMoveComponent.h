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

	// Ű�� �Է¹޾Ƽ� ������ ����� �� �������� �̵��ϰ�ʹ�.
	// ���� ��ư�� ������ �����ϰ�ʹ�.
	FVector dir;
	void OnAxisMoveForward(float value);
	void OnAxisMoveRight(float value);

	void OnActionJump();

	// ���� ����Ʈ ��ư�� ������ �޸���ʹ�. ���� �Ȱ�ʹ�.
// ������ �� �ִ�ӷ��� walkSpeed�� �ϰ�ʹ�.
	float walkSpeed = 200;
	float runSpeed = 600;
	void OnActionRunPressed();
	void OnActionRunReleased();
};
