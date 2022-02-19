// Fill out your copyright notice in the Description page of Project Settings.


#include "NewFloater.h"

// Sets default values
ANewFloater::ANewFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
}

// Called when the game starts or when spawned
void ANewFloater::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANewFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

