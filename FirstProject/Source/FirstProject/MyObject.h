// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObject.generated.h" // 리플렉션 관련 코드들 들어있음

/**
 * 
 */
UCLASS(Blueprintable) // Blueprintable해야 에디터이서 이 클래스를 베이스로 블루프린트 만들 수 있음.
class FIRSTPROJECT_API UMyObject : public UObject // OBJECT상속받는건 U붙음
{
	GENERATED_BODY()
	
public:
	UMyObject();

	UPROPERTY(BlueprintReadWrite, Category = "MyVariable") // read (get), write(set)
	float MyFloat;

	UFUNCTION(BlueprintCallable, Category = "MyFunction") // BlueprintCallable : 블루프린트에서 함수 호출 가능
	void MyFunction();
};
