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
	// SpringArmComponent, CameraComponent �� ���� ���̰�ʹ�.
	// CameraComponent�� SpringArmComponent�� �ڽ����� ���̰�ʹ�.

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* cameraComp;

	void OnAxisLookUp(float value);
	void OnAxisTurn(float value);

	// Ű�� �Է¹޾Ƽ� ������ ����� �� �������� �̵��ϰ�ʹ�.
	// ���� ��ư�� ������ �����ϰ�ʹ�.
	FVector dir;
	float walkSpeed = 600;
	void OnAxisMoveForward(float value);
	void OnAxisMoveRight(float value);

	void OnActionJump();

	// ���콺 ���� ��ư�� ������ �Ѿ˰��忡�� �Ѿ��� ���� �ѱ���ġ�� ��ġ�ϰ�ʹ�.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gunMesh;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* sniperMesh;

	void OnActionFire();


	// 1, 2 ��ư�� ������ ���� ��ü�ǵ��� �ϰ�ʹ�.
	bool bChooseGun = true;
	void OnActionChooseGun();
	void OnActionChooseSniper();

	UPROPERTY(EditAnywhere)
	class UParticleSystem* bulletImpactFactory;

	void OnActionZoomIn();
	void OnActionZoomOut();

};
