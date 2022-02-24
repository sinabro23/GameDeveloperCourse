// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"



void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}

		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment); //제일작게 할당해놔서 안보이게 미리.
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		// 월드상의 몬스터의 위치(Fvector)를 모니터 위치로 옮기기위함
		// 화면은 좌측 상단이 (0,0) 우측하단이 (1,1)임
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport); // PositionInViewport에 값이나옴
		PositionInViewport.Y -= 85.f; // 너무 위젯이 몹 한가운데 있어서 올리기위해

		FVector2D SizeInViewport(300.f, 25.f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}


void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthbarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthbarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
