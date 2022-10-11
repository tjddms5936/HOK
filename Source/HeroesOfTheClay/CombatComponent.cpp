// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "HeroCharBase.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	// ...
	UnitCharBase = Cast<AHeroCharBase>(GetOwner());
	if (UnitCharBase) {
		BattlePosition = UnitCharBase->GetActorTransform();
	}
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UActorComponent::EndPlay(EndPlayReason);
	// 델리게이트 해제
	TurnEnded.Clear();
	// EndPlay함수에 Clear()함수는 일종의 메모리 해제 함수입니다, 해당 델리게이트에 바인드된 함수를 제거합니다.
	// 멀티캐스트(Clear), 싱글캐스트(Unbind) 정도의 차이
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
	if (TurnEnded.IsBound() == true) TurnEnded.Broadcast();
	/**
		실제 델리게이트에 등록된 Callback 함수를 호출하는 부분입니다.
		IsBound()를 통해 바인드가 되어있는지 확인하고 리턴값이 True이면 Broadcast()함수를 통해 호출합니다.
		인자 값이 있는 함수는 Broadcast함수의 인자 값으로 입력합니다.
		싱글캐스트 : BroadCast -> Execute 
	*/
}

void UCombatComponent::StartUnitTurn()
{

}


void UCombatComponent::EndUnitTurn()
{

}


void UCombatComponent::BeginBattle()
{

}