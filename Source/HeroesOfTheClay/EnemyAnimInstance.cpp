// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyHeroChar.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr) {
		// Pawn�� ����Ű�� �ִ°� ������ �� �ִϸ��̼� �ν��Ͻ��� �����ڸ� �������� ������ ó��
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			EnemyCharRef = Cast<AEnemyHeroChar>(Pawn);
		}
	}
	else {
		EnemyCharRef = Cast<AEnemyHeroChar>(Pawn);
	}
}

// ���ǵ� ������Ʈ ���� �� �ִ� Custom �Լ� : �� �����Ӹ��� ȣ��� ���� �� �Լ���

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			EnemyCharRef = Cast<AEnemyHeroChar>(Pawn);
		}
	}
	else {
		FVector Speed = Pawn->GetVelocity(); // Ư�� �����ӿ��� � ���ǵ带 ������ �ִ��� ��ȯ
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		if (EnemyCharRef == nullptr) {
			EnemyCharRef = Cast<AEnemyHeroChar>(Pawn);
		}
	}
}