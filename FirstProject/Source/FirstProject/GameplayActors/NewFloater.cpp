// Fill out your copyright notice in the Description page of Project Settings.


#include "NewFloater.h"

// Sets default values
ANewFloater::ANewFloater() :
	InitialLocation(FVector(0.f)),
	PlacedLocation(FVector(0.f)),
	WorldOrigin(FVector(0.f)),
	InitialDirection(FVector(0.f)),
	bInitializeFloaterLocations(true),
	bShouldFloat(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
}

// Called when the game starts or when spawned
void ANewFloater::BeginPlay()
{
	Super::BeginPlay();

	PlacedLocation = GetActorLocation();

	if (bInitializeFloaterLocations)
	{
		//SetActorLocation(InitialLocation);
	}
}

// Called every frame
void ANewFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldFloat)
	{
		FHitResult HitResult;
		// deltalocation은 이 함수 호출될때 얼마나 움직일지
		// sweep이 true이면 스윕하는지 체크
		AddActorLocalOffset(InitialDirection, true, &HitResult);
		FVector HitLocation = HitResult.Location;
		
	}
}

