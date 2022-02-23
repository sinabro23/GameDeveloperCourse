// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Main.h"

AExplosive::AExplosive()
{
	Damage = 15.f;
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapBegin()"));

	// 오버랩된 액터 체크
	if (OtherActor)
	{
		// 액터를 AMain클래스로 캐스팅
		AMain* MainCharacter = Cast<AMain>(OtherActor);
		
		if (MainCharacter) // 캐스팅 결과 유효하지 않으면 nullptr임
		{
			MainCharacter->DecrementHealth(Damage);
			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapEnd()"));
}
