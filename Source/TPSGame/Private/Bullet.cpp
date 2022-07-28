// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("collisionComp"));
	SetRootComponent(collisionComp);
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComp->SetupAttachment(collisionComp);
	meshComp->SetRelativeLocation(FVector(0, 0, -31));
	meshComp->SetRelativeScale3D(FVector(0.625f));

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
	// �̵��Ҷ� ��ǥ�� ����ȭ�� ������Ʈ�� �����ϰ�ʹ�.
	movementComp->SetUpdatedComponent(collisionComp);
	// �̵��ϱ����� ������ �����ϰ�ʹ�. velocity  ũ��
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

