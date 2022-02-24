// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

// 무기가 장착되지도 않았는데 몬스터가 근접해서 대미지 입지 않게 하기 위해서 처음 만들었음
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup		UMETA(DisplayName = "Pickup"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),

	EWS_MAX			UMETA(DisplayName = "DefaultMax"),
};

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	bool bWeaponParticle; // 아이템 FX끄고 켤수있게

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* SwingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* SkeletalMesh; //AItem에는 staticmesh있음

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
	class UBoxComponent* CombatCollision; //무기 딜 넣을수 있는 박스콜리전

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	float Damage;



protected:

	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	void Equip(class AMain* Character);

	UFUNCTION(BlueprintCallable)
	void ActivateCollsion();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollsion();

	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }
	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }

	// 무기 대미지를위한 오버랩 함수
	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator; // ApplyDamage인자 (컨트롤러임)

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }
};
