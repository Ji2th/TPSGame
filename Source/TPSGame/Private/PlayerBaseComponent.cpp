// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseComponent.h"
#include "TPSPlayer.h"
#include "TPSGame.h"

// Sets default values for this component's properties
UPlayerBaseComponent::UPlayerBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
}


void UPlayerBaseComponent::InitializeComponent()
{
	me = Cast<ATPSPlayer>(GetOwner());

	me->onPlayerInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::PlayerInputBinding);

	PRINT_LOG(TEXT("UPlayerBaseComponent::InitializeComponent"));
}

// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	
	// ...
	PRINT_LOG(TEXT("UPlayerBaseComponent::BeginPlay"));
	
}


// Called every frame
void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

