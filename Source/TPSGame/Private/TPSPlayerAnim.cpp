// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"
#include "TPSGame.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 내 오너의 이동 Velocity를 가져와서 speedForward에 대입하고싶다.
	auto owner = TryGetPawnOwner();
	auto player = Cast<ATPSPlayer>(owner);
	if (player)
	{
		speedForward = FVector::DotProduct(owner->GetVelocity(), owner->GetActorForwardVector());
		speedRight = FVector::DotProduct(owner->GetVelocity(), owner->GetActorRightVector());

		// 공중인가?
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();

		//PRINT_LOG(TEXT("%f, %f"), speedForward, speedRight);
	}
}

void UTPSPlayerAnim::PlayAttackAnimation()
{
	Montage_Play(attackAnimMontage);
}
