// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "Bullet.h"
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>
#include "Enemy.h"
#include "EnemyFSM.h"

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
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 태어날 때 SniperUI공장에서 SniperUI를 만들어서 가지고 있고싶다.
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// 태어날 때 CrosshairUI공장에서 CrosshairUI를 만들어서 가지고 있고싶다.
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	JumpMaxCount = 2;

	OnActionChooseSniper();
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// dir방향으로 이동하고싶다.
	dir = FTransform(GetControlRotation()).TransformVector(dir);
	dir.Z = 0; // 수직으로 아래를 보더라도 이동하려면...
	dir.Normalize();
	//// P = P0 + vt
	//FVector velocity = dir * walkSpeed;
	//SetActorLocation(GetActorLocation() + velocity * DeltaTime);

	AddMovementInput(dir);

	dir = FVector::ZeroVector;

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::OnAxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::OnAxisTurn);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATPSPlayer::OnAxisMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATPSPlayer::OnAxisMoveRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::OnActionJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::OnActionFire);
	PlayerInputComponent->BindAction(TEXT("ChooseGun"), IE_Pressed, this, &ATPSPlayer::OnActionChooseGun);
	PlayerInputComponent->BindAction(TEXT("ChooseSniper"), IE_Pressed, this, &ATPSPlayer::OnActionChooseSniper);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ATPSPlayer::OnActionZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ATPSPlayer::OnActionZoomOut);
}

void ATPSPlayer::OnAxisLookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::OnAxisTurn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::OnAxisMoveForward(float value) // -1 1
{
	// 입력받은 value를 dir의 X값에 대입하고싶다.
	// lValue : dir
	// rValue : value
	dir.X = value;
}

void ATPSPlayer::OnAxisMoveRight(float value) // -1 1
{
	// 입력받은 value를 dir의 Y값에 대입하고싶다.
	dir.Y = value;
}

void ATPSPlayer::OnActionJump()
{
	Jump();
}

void ATPSPlayer::OnActionFire()
{
	if (bChooseGun)
	{
		// gun을 고른 상태에서 총을 쏘고싶다.
		FTransform firePosition = gunMesh->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else
	{
		// sniper를 고른 상태에서 총을 쏘고싶다. 
		// 시작점 끝점
		FVector start = cameraComp->GetComponentLocation();
		FVector end = start + cameraComp->GetForwardVector() * 100000;
		// 부딪힌곳의 정보
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, params);

		if (isHit)
		{
			// 총알자국공장에서 총알자국을 SpawnEmitter하고 부딪힌 그곳에 배치하고싶다.
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, hitInfo.ImpactPoint);

			// 만약에 부딪힌 물체가 물리를 작용하고 있다면 그물체에게 힘을 가하고싶다.
			auto hitComp = hitInfo.GetComponent();
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				hitComp->AddForceAtLocation(hitComp->GetMass() * -hitInfo.ImpactNormal * 100000, hitInfo.ImpactPoint);
			}

			// 만약 부딪힌 상대가 AEnemy라면
			auto enemy = Cast<AEnemy>(hitInfo.GetActor());
			if (nullptr != enemy)
			{
				// AEnemy의 OnTakeDamage()를 호출하고싶다.
				auto comp = enemy->GetComponentByClass(UEnemyFSM::StaticClass());
				auto fsm = Cast<UEnemyFSM>(comp);

				fsm->OnTakeDamage();
			}
		}
	}
}
void ATPSPlayer::OnActionChooseGun()
{
	// gun만 보이게 하고싶다.
	bChooseGun = true;
	gunMesh->SetVisibility(true);
	sniperMesh->SetVisibility(false);
	cameraComp->SetFieldOfView(90);

	crosshairUI->RemoveFromParent();
	sniperUI->RemoveFromViewport();
}
void ATPSPlayer::OnActionChooseSniper()
{
	// sniper만 보이게 하고싶다.
	bChooseGun = false;
	gunMesh->SetVisibility(false);
	sniperMesh->SetVisibility(true);
	// Sniper를 선택하면 CrosshairUI를 보이게 하고싶다.
	crosshairUI->AddToViewport();
}

void ATPSPlayer::OnActionZoomIn()
{
	// 만약 sniper라면
	if (false == bChooseGun)
	{
		cameraComp->SetFieldOfView(30);
		// ZoomIn : SniperUI를 보이게 하고싶다. 
		sniperUI->AddToViewport();
		crosshairUI->RemoveFromParent();
	}
}

void ATPSPlayer::OnActionZoomOut()
{
	if (false == bChooseGun)
	{
		cameraComp->SetFieldOfView(90);
		// ZoomOut : SniperUI를 보이지 않게 하고싶다. 
		sniperUI->RemoveFromParent();
		crosshairUI->AddToViewport();
	}
}

