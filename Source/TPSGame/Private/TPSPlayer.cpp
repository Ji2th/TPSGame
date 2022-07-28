// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "Bullet.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// SpringArmComponent�� ���� ROOT�� ���̰�ʹ�.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->TargetArmLength = 200;
	springArmComp->SetRelativeLocation(FVector(0, 35, 80));

	// CameraComponent�� ���� springArmComp�� �ڽ����� ���̰�ʹ�.
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArmComp);


	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = false;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));

	if (tempMesh.Succeeded())
	{
		// �о���� USkeletalMesh�� USkeletalMeshComponent�� �����ϰ�ʹ�.
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}


	// gunMesh�� �����ϰ�ʹ�.
	gunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMesh"));
	gunMesh->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGunMesh.Succeeded())
	{
		gunMesh->SetSkeletalMesh(tempGunMesh.Object);
		gunMesh->SetRelativeLocation(FVector(-14, 52, 120));
	}

	// sniperMesh�� �����ϰ�ʹ�.
	sniperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperMesh"));
	sniperMesh->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));

	if (tempSniperMesh.Succeeded())
	{
		sniperMesh->SetStaticMesh(tempSniperMesh.Object);
		sniperMesh->SetRelativeLocation(FVector(-22, 55, 120));
		sniperMesh->SetRelativeScale3D(FVector(0.15f));
	}

}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	OnActionChooseGun();
	JumpMaxCount = 2;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// dir�������� �̵��ϰ�ʹ�.
	dir = FTransform(GetControlRotation()).TransformVector(dir);
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
	// �Է¹��� value�� dir�� X���� �����ϰ�ʹ�.
	// lValue : dir
	// rValue : value
	dir.X = value;
}

void ATPSPlayer::OnAxisMoveRight(float value) // -1 1
{
	// �Է¹��� value�� dir�� Y���� �����ϰ�ʹ�.
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
		// gun�� �� ���¿��� ���� ���ʹ�.
		FTransform firePosition = gunMesh->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else
	{
		// sniper�� �� ���¿��� ���� ���ʹ�. 
		// ������ ����
		FVector start = cameraComp->GetComponentLocation();
		FVector end = start + cameraComp->GetForwardVector() * 100000;
		// �ε������� ����
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, params);

		if (isHit)
		{
			// �Ѿ��ڱ����忡�� �Ѿ��ڱ��� SpawnEmitter�ϰ� �ε��� �װ��� ��ġ�ϰ�ʹ�.
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, hitInfo.ImpactPoint);

			// ���࿡ �ε��� ��ü�� ������ �ۿ��ϰ� �ִٸ� �׹�ü���� ���� ���ϰ�ʹ�.
			auto hitComp = hitInfo.GetComponent();
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				hitComp->AddForce(hitComp->GetMass() * -hitInfo.ImpactNormal * 100000);
			}


		}

	}
}
void ATPSPlayer::OnActionChooseGun()
{
	// gun�� ���̰� �ϰ�ʹ�.
	bChooseGun = true;
	gunMesh->SetVisibility(true);
	sniperMesh->SetVisibility(false);
	cameraComp->SetFieldOfView(90);

}
void ATPSPlayer::OnActionChooseSniper()
{
	// sniper�� ���̰� �ϰ�ʹ�.
	bChooseGun = false;
	gunMesh->SetVisibility(false);
	sniperMesh->SetVisibility(true);
}

void ATPSPlayer::OnActionZoomIn()
{
	// ���� sniper���
	if (false == bChooseGun)
	{
		cameraComp->SetFieldOfView(30);
	}
}

void ATPSPlayer::OnActionZoomOut()
{
	if (false == bChooseGun)
	{
		cameraComp->SetFieldOfView(90);
	}
}

