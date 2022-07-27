// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	// SpringArmComponent를 만들어서 ROOT에 붙이고싶다.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->TargetArmLength = 400;

	// CameraComponent을 만들어서 springArmComp의 자식으로 붙이고싶다.
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArmComp);


	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	JumpMaxCount = 2;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// dir방향으로 이동하고싶다.
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
}

void ATPSPlayer::OnAxisLookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::OnAxisTurn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::OnAxisMoveForward(float value) // -1 0 1
{
	// 입력받은 value를 dir의 X값에 대입하고싶다.
	// lValue : dir
	// rValue : value
	dir.X = value;
}

void ATPSPlayer::OnAxisMoveRight(float value) // -1 0 1
{
	// 입력받은 value를 dir의 Y값에 대입하고싶다.
	dir.Y = value;
}

void ATPSPlayer::OnActionJump()
{
	Jump();
}

