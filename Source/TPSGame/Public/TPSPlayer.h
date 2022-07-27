// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSGAME_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// SpringArmComponent, CameraComponent 를 만들어서 붙이고싶다.
	// CameraComponent을 SpringArmComponent의 자식으로 붙이고싶다.

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* cameraComp;

	void OnAxisLookUp(float value);
	void OnAxisTurn(float value);

	// 키를 입력받아서 방향을 만들고 그 방향으로 이동하고싶다.
	// 점프 버튼을 누르면 점프하고싶다.
	FVector dir;
	float walkSpeed = 600;
	void OnAxisMoveForward(float value);
	void OnAxisMoveRight(float value);

	void OnActionJump();
};
