// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

// UE4 C++ ���� ��� ������ ��������Ʈ ���� ������ �� 4���� : �̱�,��Ƽĳ��Ʈ,�̺�Ʈ,���̳���
// �� �� �̺�Ʈ�� Delegate�� �޸� Event�� Ŭ���� ���ο� �ۼ��ϰ� ���ڰ����� ����Ŭ������ Ŭ������ �Է��մϴ�. �ܿ��� ���� ��Ƽ�ɽ�Ʈ�� ������ �����մϴ�.

// ���̳����� ��찡 �������Ʈ�� �̺�Ʈ �����Ķ� ����ϰ� ����� ����
// ��������Ʈ ���� �ڼ��ϰ� ���� ���� �ڷ� : https://darkcatgame.tistory.com/66

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnEnded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEROESOFTHECLAY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FTurnEnded TurnEnded;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom Variables")
	class AHeroCharBase* UnitCharBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom Variables")
	FTransform BattlePosition;

	UFUNCTION(BlueprintCallable)
	void StartUnitTurn();

	UFUNCTION(BlueprintCallable)
	void EndUnitTurn();

	UFUNCTION(BlueprintCallable)
	void BeginBattle();
};
