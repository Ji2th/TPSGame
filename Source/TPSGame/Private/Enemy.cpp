// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));

	if (tempMesh.Succeeded())
	{
		// 읽어들인 USkeletalMesh를 USkeletalMeshComponent에 적용하고싶다.
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}

	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("fsm"));

	ConstructorHelpers::FClassFinder<UEnemyAnim> tempEnemyAnim(TEXT("AnimBlueprint'/Game/Animations/AB_Enemy.AB_Enemy_C'"));

	if (tempEnemyAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempEnemyAnim.Class);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

