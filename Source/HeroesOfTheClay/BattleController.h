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

	// ������ �ǽð� ü�¹� ���� -> �ϵ��ڵ��Ϸ��� �����۾��� �������Ʈ�� ������ ���Ƽ� �׳� �������Ʈ���� ��.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HealthBar;
protected:

	virtual void BeginPlay() override;
};
