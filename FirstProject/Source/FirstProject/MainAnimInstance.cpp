// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UMainAnimInstance::UMainAnimInstance()
{
	// 폰이 이미 있어야 애님인스턴스가 가능한건데 생성자에서 하면 시기가 이상함.

}

void UMainAnimInstance::NativeInitializeAnimation()
{
	if (nullptr == Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (nullptr == Pawn)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
	}

}

