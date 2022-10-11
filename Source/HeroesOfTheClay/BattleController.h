// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattleController.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFTHECLAY_API ABattleController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABattleController();

	// 전투시 실시간 체력바 위젯 -> 하드코딩하려다 위젯작업은 블루프린트가 나은거 같아서 그냥 블루프린트에서 함.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HealthBar;
protected:

	virtual void BeginPlay() override;
};
