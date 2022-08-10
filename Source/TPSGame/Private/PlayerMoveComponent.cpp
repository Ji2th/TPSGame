// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "TPSGame.h"

UPlayerMoveComponent::UPlayerMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// 시작할 때 최대속력을 walkSpeed로 하고싶다.
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// dir방향으로 이동하고싶다.
	dir = FTransform(me->GetControlRotation()).TransformVector(dir);
	dir.Z = 0; // 수직으로 아래를 보더라도 이동하려면...
	dir.Normalize();

	me->AddMovementInput(dir);

	dir = FVector::ZeroVector;
}

void UPlayerMoveComponent::PlayerInputBinding(class UInputComponent* PlayerInputComponent)
{
	PRINT_LOG(TEXT("UPlayerMoveComponent::PlayerInputBinding"));
	Super::PlayerInputBinding(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMoveComponent::OnAxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMoveComponent::OnAxisTurn);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &UPlayerMoveComponent::OnAxisMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &UPlayerMoveComponent::OnAxisMoveRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMoveComponent::OnActionJump);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMoveComponent::OnActionRunPressed);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMoveComponent::OnActionRunReleased);
}


void UPlayerMoveComponent::OnAxisLookUp(float value)
{
	me->AddControllerPitchInput(value);
}

void UPlayerMoveComponent::OnAxisTurn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerMoveComponent::OnAxisMoveForward(float value) // -1 1
{
	// 입력받은 value를 dir의 X값에 대입하고싶다.
	// lValue : dir
	// rValue : value
	dir.X = value;
}

void UPlayerMoveComponent::OnAxisMoveRight(float value) // -1 1
{
	// 입력받은 value를 dir의 Y값에 대입하고싶다.
	dir.Y = value;
}

void UPlayerMoveComponent::OnActionJump()
{
	me->Jump();
}


void UPlayerMoveComponent::OnActionRunPressed()
{
	me->GetCharacterMovement()->MaxWalkSpeed = runSpeed;

}

void UPlayerMoveComponent::OnActionRunReleased()
{
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

}
