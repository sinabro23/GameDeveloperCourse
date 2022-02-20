// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class FIRSTPROJECT_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UColliderMovementComponent* OurMovementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

public:
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return Mesh; }
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* MeshComponent) { Mesh = MeshComponent; }

	FORCEINLINE USphereComponent* GetSphereComponent() { return Sphere; }
	FORCEINLINE void SetSphereComponent(USphereComponent* SphereComponent) { Sphere = SphereComponent; }

	FORCEINLINE UCameraComponent* GetCameraComponent() { return Camera; }
	FORCEINLINE void SetCameraComponent(UCameraComponent* CameraComponent) { Camera = CameraComponent; }

	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return SpringArm; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* SpringArmComponent) { SpringArm = SpringArmComponent; }



private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void YawCamera(float Value);
	void PitchCamera(float Value);

	FVector2D CameraIput;

};
