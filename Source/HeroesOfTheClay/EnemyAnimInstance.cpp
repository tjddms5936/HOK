// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyHeroChar.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr) {
		// Pawn이 가리키고 있는게 없으면 이 애니매이션 인스턴스의 소유자를 가져오는 유용한 처리
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			EnemyCharRef = Cast<AEnemyHeroChar>(Pawn);
		}
	}
	else {
		EnemyCharRef = Cast<AEnemyHeroChar>(Pawn);
	}
}

// 스피드 업데이트 해줄 수 있는 Custom 함수 : 매 프레임마다 호출될 것임 이 함수는

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			EnemyCharRef = Cast<AEnemyHeroChar>(Pawn);
		}
	}
	else {
		FVector Speed = Pawn->GetVelocity(); // 특정 프레임에서 어떤 스피드를 가지고 있는지 반환
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		if (EnemyCharRef == nullptr) {
			EnemyCharRef = Cast<AEnemyHeroChar>(Pawn);
		}
	}
}