// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TPSGAME_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 구 형태의 총알을 제작하고싶다.
	UPROPERTY(EditAnywhere)
	class USphereComponent* collisionComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	// 발사체이동관련 컴포넌트를 이용해서 이동하고싶다.
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* movementComp;
};
