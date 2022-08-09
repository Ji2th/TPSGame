// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"
#include "TPSPlayer.h"
#include <Blueprint/UserWidget.h>
#include "TPSPlayerAnim.h"
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"
#include "EnemyFSM.h"
#include "Bullet.h"

void UPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();

	// 태어날 때 SniperUI공장에서 SniperUI를 만들어서 가지고 있고싶다.
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// 태어날 때 CrosshairUI공장에서 CrosshairUI를 만들어서 가지고 있고싶다.
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	me->JumpMaxCount = 2;

	OnActionChooseSniper();
}

void UPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerFireComponent::PlayerInputBinding(class UInputComponent* PlayerInputComponent)
{
	Super::PlayerInputBinding(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFireComponent::OnActionFire);
	PlayerInputComponent->BindAction(TEXT("ChooseGun"), IE_Pressed, this, &UPlayerFireComponent::OnActionChooseGun);
	PlayerInputComponent->BindAction(TEXT("ChooseSniper"), IE_Pressed, this, &UPlayerFireComponent::OnActionChooseSniper);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &UPlayerFireComponent::OnActionZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &UPlayerFireComponent::OnActionZoomOut);
}




void UPlayerFireComponent::OnActionFire()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(cameraShake);

	// 메시로부터 애니메이션인스턴스를 가져와서 공격애니메이션을 호출하고싶다.
	auto anim = Cast<UTPSPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnimation();

	if (bChooseGun)
	{
		// gun을 고른 상태에서 총을 쏘고싶다.
		FTransform firePosition = me->gunMesh->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else
	{
		// sniper를 고른 상태에서 총을 쏘고싶다. 
		// 시작점 끝점
		FVector start = me->cameraComp->GetComponentLocation();
		FVector end = start + me->cameraComp->GetForwardVector() * 100000;
		// 부딪힌곳의 정보
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(me);

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
void UPlayerFireComponent::OnActionChooseGun()
{
	// gun만 보이게 하고싶다.
	bChooseGun = true;
	me->gunMesh->SetVisibility(true);
	me->sniperMesh->SetVisibility(false);
	me->cameraComp->SetFieldOfView(90);

	crosshairUI->RemoveFromParent();
	sniperUI->RemoveFromViewport();
}
void UPlayerFireComponent::OnActionChooseSniper()
{
	// sniper만 보이게 하고싶다.
	bChooseGun = false;
	me->gunMesh->SetVisibility(false);
	me->sniperMesh->SetVisibility(true);
	// Sniper를 선택하면 CrosshairUI를 보이게 하고싶다.
	crosshairUI->AddToViewport();
}

void UPlayerFireComponent::OnActionZoomIn()
{
	// 만약 sniper라면
	if (false == bChooseGun)
	{
		me->cameraComp->SetFieldOfView(30);
		// ZoomIn : SniperUI를 보이게 하고싶다. 
		sniperUI->AddToViewport();
		crosshairUI->RemoveFromParent();
	}
}

void UPlayerFireComponent::OnActionZoomOut()
{
	if (false == bChooseGun)
	{
		me->cameraComp->SetFieldOfView(90);
		// ZoomOut : SniperUI를 보이지 않게 하고싶다. 
		sniperUI->RemoveFromParent();
		crosshairUI->AddToViewport();
	}
}
