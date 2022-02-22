// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	FVector StartPoint; // ���ٴϴ� �޽��� ù����

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	FVector EndPoint; // �޽��� ������, meta = (MakeEditWidget = "true")�̷��� �ϸ� ���忡 ����Ʈ ���׶��� ��ġ������.
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpSpeed; // ��ȯ �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpTime; // ���������� ���� �ð�

	FTimerHandle InterpTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool bInterping;

	float Distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();

	// ���� ���Ϳ� �����͸� �ٲٰ� �ϱ� ����
	void SwapVectors(FVector& VecOne, FVector& VecTwo);
};