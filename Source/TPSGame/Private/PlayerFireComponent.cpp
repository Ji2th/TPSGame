// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"

void UPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();//<-�̰ɷ� �����ذ�

	UE_LOG(LogTemp, Warning, TEXT("UPlayerFireComponent"));
}

void UPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void UPlayerFireComponent::PlayerInputBinding(class UInputComponent* PlayerInputComponent)
{

}
