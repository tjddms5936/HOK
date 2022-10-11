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
	// ��������Ʈ ����
	TurnEnded.Clear();
	// EndPlay�Լ��� Clear()�Լ��� ������ �޸� ���� �Լ��Դϴ�, �ش� ��������Ʈ�� ���ε�� �Լ��� �����մϴ�.
	// ��Ƽĳ��Ʈ(Clear), �̱�ĳ��Ʈ(Unbind) ������ ����
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
	if (TurnEnded.IsBound() == true) TurnEnded.Broadcast();
	/**
		���� ��������Ʈ�� ��ϵ� Callback �Լ��� ȣ���ϴ� �κ��Դϴ�.
		IsBound()�� ���� ���ε尡 �Ǿ��ִ��� Ȯ���ϰ� ���ϰ��� True�̸� Broadcast()�Լ��� ���� ȣ���մϴ�.
		���� ���� �ִ� �Լ��� Broadcast�Լ��� ���� ������ �Է��մϴ�.
		�̱�ĳ��Ʈ : BroadCast -> Execute 
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