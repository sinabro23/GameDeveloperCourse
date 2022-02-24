// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainPlayerController.h"

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

	MaxHealth = 100.f;
	Health = 65.f;
	MaxStamina = 150.f;
	Stamina = 120.f;
	Coins = 0;

	RunningSpeed = 650.f;
	SpringtingSpeed = 950.f;

	bShiftKeyDown = false;
	bLMBDown = false;


	// Initialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 25.f; // 초당 떨어지는 스태미나
	MinSprintStamina = 50.f;

	bAttacking = false;

	InterpSpeed = 15.f;
	bInterToEnemy = false;

	bHasCombatTarget = false;
}


void AMain::ShowPickupLocations()
{
	for (FVector Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(
			this,
			Location,
			25.f,
			8,
			FLinearColor::Green,
			10.f,
			.5f);
	}
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaDrainRate * DeltaTime; // 줄어드는지 늘어나는지 상관없이 일정값

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting); // 스피드 바꾸는것
		}
		else // 쉬프트키 떼면
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0.f;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else // 쉬프트키 떼면
		{
			if (Stamina += DeltaStamina >= MinSprintStamina)
			{
				Stamina += DeltaStamina;
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.f;
		}
		else // 쉬프트키 떼면
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina += DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		break;
	}

	if (bInterToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed); // R은 Rotator

		SetActorRotation(InterpRotation);
	}

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation; // Set함수로 해야할것 같은데.
		}
	}
}

FRotator AMain::GetLookAtRotationYaw(FVector Target) // 내위치에서 타겟위치를 바라보는 로테이터를 구함
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f); // 고개돌리기위해 Yaw값만 필요함
	return LookAtRotationYaw;
}


// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AMain::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMain::ShiftKeyUp);
	PlayerInputComponent->BindAction(TEXT("LMB"), EInputEvent::IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction(TEXT("LMB"), EInputEvent::IE_Released, this, &AMain::LMBUp);
	
		
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMain::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AMain::LookupAtRate);
	


}

void AMain::MoveForward(float Value)
{
	if (Controller && 0 != Value && !bAttacking)
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
	if (Controller && 0 != Value && !bAttacking)
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


void AMain::DecrementHealth(float Amount)
{

	if (Health - Amount <= 0.f)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Health -= Amount;
	}

}


float AMain::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DecrementHealth(DamageAmount);

	return DamageAmount;
}

void AMain::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
}

void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;

	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SpringtingSpeed;
	}
	else 
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMain::LMBDown()
{
	bLMBDown = true;
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr); // 무기를 집고나서도 ActiveOverlappingItem가 null이 안됐었음
		}
	}
	else if(EquippedWeapon) // 무기를 장착하고 바다에 떨어진 무기범위 안이 아니라면
	{
		Attack();
	}
}

void AMain::LMBUp()
{
	bLMBDown = false;
}


void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon) // 기존에 장착한 무기가 있다면 파괴하라
	{
		EquippedWeapon->Destroy();
	}

	EquippedWeapon = WeaponToSet;
}

void AMain::Attack()
{
	if (!bAttacking) // 이미 공격하고있어도 왼클릭누르면 처음부터 다시공격하는거 방지
	{
		bAttacking = true;
		SetInterpToEnemy(true);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0, 1); // 랜덤으로 몽타주 섹션 1과 2를 번갈아가면서 쓰게하기 위한 변수
			switch (Section)
			{
			case 0:
				// 두번째 인자 몽타주 플레이 속도 ex) 1.35f는 35%빠르게
				AnimInstance->Montage_Play(CombatMontage, 2.2f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage); // Default지우고 만든 섹션
				break;
			case 1:
				// 두번째 인자 몽타주 플레이 속도 1.35f는 35%빠르게
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage); // Default지우고 만든 섹션
				break;
			default:
				break;
			}
			
		}

	}
}

void AMain::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	if (bLMBDown) // 왼버튼 누르고 있으면 계속 공격하게
	{
		Attack();
	}
}

void AMain::PlaySwingSound() // 애님노티파이로 애님BP에서 호출될 함수
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}

}
void AMain::SetInterpToEnemy(bool Interp)
{
	bInterToEnemy = Interp;
}