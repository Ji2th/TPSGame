// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

UPlayerMoveComponent::UPlayerMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ������ �� �ִ�ӷ��� walkSpeed�� �ϰ�ʹ�.
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// dir�������� �̵��ϰ�ʹ�.
	dir = FTransform(me->GetControlRotation()).TransformVector(dir);
	dir.Z = 0; // �������� �Ʒ��� ������ �̵��Ϸ���...
	dir.Normalize();

	me->AddMovementInput(dir);

	dir = FVector::ZeroVector;
}

void UPlayerMoveComponent::PlayerInputBinding(class UInputComponent* PlayerInputComponent)
{
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
	// �Է¹��� value�� dir�� X���� �����ϰ�ʹ�.
	// lValue : dir
	// rValue : value
	dir.X = value;
}

void UPlayerMoveComponent::OnAxisMoveRight(float value) // -1 1
{
	// �Է¹��� value�� dir�� Y���� �����ϰ�ʹ�.
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
