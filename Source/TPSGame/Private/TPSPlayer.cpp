// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "PlayerMoveComponent.h"
#include "PlayerFireComponent.h"
#include "TPSGame.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// SpringArmComponent를 만들어서 ROOT에 붙이고싶다.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->TargetArmLength = 200;
	springArmComp->SetRelativeLocation(FVector(0, 35, 80));

	// CameraComponent을 만들어서 springArmComp의 자식으로 붙이고싶다.
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArmComp);


	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = false;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));

	if (tempMesh.Succeeded())
	{
		// 읽어들인 USkeletalMesh를 USkeletalMeshComponent에 적용하고싶다.
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}


	// gunMesh를 설정하고싶다.
	gunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMesh"));
	gunMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGunMesh.Succeeded())
	{
		gunMesh->SetSkeletalMesh(tempGunMesh.Object);
		gunMesh->SetRelativeLocationAndRotation(FVector(-17, 10, -3), FRotator(0, 90, 0));
	}

	// sniperMesh를 설정하고싶다.
	sniperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperMesh"));
	sniperMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));

	if (tempSniperMesh.Succeeded())
	{
		sniperMesh->SetStaticMesh(tempSniperMesh.Object);
		sniperMesh->SetRelativeLocationAndRotation(FVector(-47, 7, 1), FRotator(0, 90, 0));
		sniperMesh->SetRelativeScale3D(FVector(0.15f));
	}

	moveComp = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("moveComp"));
	fireComp = CreateDefaultSubobject<UPlayerFireComponent>(TEXT("fireComp"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	// 시작할 때 체력을 최대체력으로 하고싶다.
	hp = maxHP;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	moveComp->PlayerInputBinding(PlayerInputComponent);
	fireComp->PlayerInputBinding(PlayerInputComponent);
}

void ATPSPlayer::OnHitEvent()
{
	// 체력을 1 감소하고싶다.
	hp--;
	PRINT_LOG(TEXT("OnHitEvent : %d"), hp);
	// 만약 체력이 0이하라면
	if (hp <= 0)
	{
		// 게임오버를 로그로 출력!
		PRINT_LOG(TEXT("Player is dead!!!!"));
		OnGameOver();
	}
}

void ATPSPlayer::OnGameOver_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

