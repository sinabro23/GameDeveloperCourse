// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = TriggerBox;

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f));
	TriggerBox->SetGenerateOverlapEvents(true);

	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSitch"));
	FloorSwitch->SetupAttachment(GetRootComponent());

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(GetRootComponent());

	DoorCloseTime = 2.f;
	bIsOnSwitch = false;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

	InitialDoorLocation = DoorMesh->GetComponentLocation();
	InitialSwitchLocation = TriggerBox->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(false == bIsOnSwitch)
		bIsOnSwitch = true;

	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	RaiseDoor();
	LowerSwitch();
}

void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsOnSwitch)
		bIsOnSwitch = false;

	UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
	GetWorldTimerManager().SetTimer(SwitchTimer, this, &AFloorSwitch::CloseDoor, DoorCloseTime);
}

void AFloorSwitch::UpdateDoorLocation(float Z)
{
	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += Z;
	DoorMesh->SetWorldLocation(NewLocation);
}

void AFloorSwitch::UpdateSwitchLocation(float Z)
{
	FVector NewLocation = InitialSwitchLocation;
	NewLocation.Z += Z;
	TriggerBox->SetWorldLocation(NewLocation);
}

void AFloorSwitch::CloseDoor()
{
	if (!bIsOnSwitch)
	{
		LowerDoor();
		RaiseSwitch();
	}
}

