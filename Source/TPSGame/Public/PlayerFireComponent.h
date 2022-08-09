// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFireComponent.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API UPlayerFireComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PlayerInputBinding(class UInputComponent* PlayerInputComponent) override;


	// 마우스 왼쪽 버튼을 누르면 총알공장에서 총알을 만들어서 총구위치에 배치하고싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

	void OnActionFire();


	// 1, 2 버튼을 누르면 총이 교체되도록 하고싶다.
	bool bChooseGun = true;
	void OnActionChooseGun();
	void OnActionChooseSniper();

	UPROPERTY(EditAnywhere)
	class UParticleSystem* bulletImpactFactory;

	void OnActionZoomIn();
	void OnActionZoomOut();

	// 태어날 때 SniperUI공장에서 SniperUI를 만들어서 가지고 있고싶다.
	// ZoomIn : SniperUI를 보이게 하고싶다. 
	// ZoomOut : SniperUI를 보이지 않게 하고싶다. 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	UPROPERTY()
	class UUserWidget* sniperUI;

	// 태어날 때 CrosshairUI공장에서 CrosshairUI를 만들어서 가지고 있고싶다.
	// Sniper를 선택하면 CrosshairUI를 보이게 하고싶다.
	// ZoomIn : CrosshairUI를 보이지 않게 하고싶다. 
	// ZoomOut : CrosshairUI를 보이게 하고싶다. 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> crosshairUIFactory= nullptr;

	UPROPERTY()
	class UUserWidget* crosshairUI;

	// 총을 쏘면 카메라를 흔들고싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> cameraShake;
};
