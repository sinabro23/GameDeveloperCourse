// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.f, -101.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_CastleGuard(TEXT("SkeletalMesh'/Game/_Game/Assets/Character/Mesh/castle_guard_01.castle_guard_01'"));
	if (SM_CastleGuard.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_CastleGuard.Object);
	}
	
	GetCapsuleComponent()->SetCapsuleSize(48.f, 105.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // 카메라붐이 로테이션하니깐 카메라는 돌면안됨.

	BaseTurnRate = 65.f;
	BaseLookupRate = 65.f;

	// 컨트롤러 따라 메쉬 안돌게만들기
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 컨트롤러 방향에따라 캐릭터 돌게 만들기
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AMain::StopJumping);
	
		
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMain::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AMain::LookupAtRate);
	


}

void AMain::MoveForward(float Value)
{
	if (Controller && 0 != Value)
	{
		// 정면기준으로 앞으로가야하고
		const FRotator ControllerRotator = Controller->GetControlRotation();
		const FRotator RotatorYaw = FRotator(0.f, ControllerRotator.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(RotatorYaw).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AMain::MoveRight(float Value)
{
	if (Controller && 0 != Value)
	{
		// 정면기준으로 오른쪽으로 가야하니깐
		const FRotator ControllerRotator = Controller->GetControlRotation();
		const FRotator RotatorYaw = FRotator(0.f, ControllerRotator.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(RotatorYaw).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookupAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookupRate* GetWorld()->GetDeltaSeconds());
}

