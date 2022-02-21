// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UMainAnimInstance::UMainAnimInstance()
{
	// ���� �̹� �־�� �ִ��ν��Ͻ��� �����Ѱǵ� �����ڿ��� �ϸ� �ñⰡ �̻���.

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
