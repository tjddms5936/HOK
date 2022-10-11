// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimInstance.h"
#include "MyHeroChar.h"


void UHeroAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr) {
		// Pawn�� ����Ű�� �ִ°� ������ �� �ִϸ��̼� �ν��Ͻ��� �����ڸ� �������� ������ ó��
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			MyHeroCharRef = Cast<AMyHeroChar>(Pawn);
		}
	}
	else {
		MyHeroCharRef = Cast<AMyHeroChar>(Pawn);
	}
}

// ���ǵ� ������Ʈ ���� �� �ִ� Custom �Լ� : �� �����Ӹ��� ȣ��� ���� �� �Լ���

void UHeroAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			MyHeroCharRef = Cast<AMyHeroChar>(Pawn);
		}
	}
	else {
		FVector Speed = Pawn->GetVelocity(); // Ư�� �����ӿ��� � ���ǵ带 ������ �ִ��� ��ȯ
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		if (MyHeroCharRef == nullptr) {
			MyHeroCharRef = Cast<AMyHeroChar>(Pawn);
		}
	}
}
