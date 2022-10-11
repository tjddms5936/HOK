// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

// UE4 C++ 에서 사용 가능한 델리게이트 세팅 종류는 총 4가지 : 싱글,멀티캐스트,이벤트,다이나믹
// 이 중 이벤트는 Delegate와 달리 Event는 클래스 내부에 작성하고 인자값으로 주인클래스의 클래스를 입력합니다. 외에는 이전 멀티케스트와 사용법이 동일합니다.

// 다이나믹의 경우가 블루프린트의 이벤트 디스패쳐랑 비슷하게 사용이 가능
// 델리게이트 관련 자세하고 쉬운 설명 자료 : https://darkcatgame.tistory.com/66

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
