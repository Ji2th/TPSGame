// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(OnPlayerInputBindingDelegate, class UInputComponent*);


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

	OnPlayerInputBindingDelegate onPlayerInputBindingDelegate;

	// SpringArmComponent, CameraComponent 를 만들어서 붙이고싶다.
	// CameraComponent을 SpringArmComponent의 자식으로 붙이고싶다.

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gunMesh;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* sniperMesh;


	UPROPERTY(EditAnywhere)
	class UPlayerBaseComponent* moveComp;

	UPROPERTY(EditAnywhere)
	class UPlayerBaseComponent* fireComp;

	// 체력을 만들어서 처리하고싶다.
	// Enemy가 Player를 공격하면 체력을 1 소모하고싶다.
	// 체력이 0이하가되면 게임오버를 화면에 출력하고 일시정지 하고싶다.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int hp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int maxHP = 10;

	UFUNCTION(BlueprintCallable)
	void OnHitEvent();

	UFUNCTION(BlueprintNativeEvent)
	void OnGameOver();

	UFUNCTION(BlueprintImplementableEvent)
	void OnChooseGun(bool bGrenadeGun);
};
