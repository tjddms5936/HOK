// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHeroChar.h"
#include "EnemyHeroChar.h"
#include "AIController.h"
#include "Components/SphereComponent.h"
#include "BattleGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"


AMyHeroChar::AMyHeroChar()
{
	// CombatStartCollision->InitSphereRadius(15.f);
	IsMyDeckSelected = false;
	IsJustSpawnedForDeck = false;
}


void AMyHeroChar::BeginPlay()
{
	Super::BeginPlay();
	if (!AIController) {
		AIController = Cast<AAIController>(GetController());
	}
}

void AMyHeroChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Target을 돌아보는 보간
	if (CombatTarget && GetHeroMovementStatus() != EHeroMovementStatus::EMS_Dead) {
		// 위 두가지 조건이 만족하면 보간 시작 : 내가 적을 바라보는 보간 
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, 15.f);

		SetActorRotation(InterpRotation);
	}
}

void AMyHeroChar::MoveToEnemy(AEnemyHeroChar* TargetEnemy)
{
	SetHeroMovementStatus(EHeroMovementStatus::EMS_MoveToTarget);
	if (AIController) {
		// AI로 Move To 하는 작업은 내부적으로 복잡하다 이해가 안가면 un4 문서 참조할 것 

		// 이동에 대한 세부 정보
		FAIMoveRequest MoveRequest;
		// 아래 두개를 제외한 나머지는 Default
		MoveRequest.SetGoalActor(TargetEnemy);
		if (bIsShortDistacne == true) {
			// 근거리 캐릭터라면
			MoveRequest.SetAcceptanceRadius(70.0f); // 캐릭과 10unit거리가 되면 이동 완료로 간주
		}
		else {
			// 원거리 캐릭터라면
			MoveRequest.SetAcceptanceRadius(500.0f); // 캐릭과 70unit거리가 되면 이동 완료로 간주
		}
		

		// 할당된 경로로 채워진 선택적 출력 매개변수
		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}
}




FRotator AMyHeroChar::GetLookAtRotationYaw(FVector Target)
{
	// FindLookAtRotation : Find a rotation for an object at Start location to point at Target location.
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AMyHeroChar::MoveToCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(OtherActor);
		if (Enemy) {
			EnemyArray.Add(Enemy);
			// UpdateCombatTarget();
		}
	}
}

void AMyHeroChar::MoveToCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 멈출 필요가 있을까? ㅇㅇ.. 다 죽으면 ! 
	
}

void AMyHeroChar::CombatCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(OtherActor);
		if (Enemy == CombatTarget) {
			SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
			if (AIController) {
				// AIController->StopMovement();
				// UE_LOG(LogTemp, Warning, TEXT("StopMovement!! From MyHero"));
			}
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AMyHeroChar::AttackStart, AttackTime);
		}
	}
}

void AMyHeroChar::CombatCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor) {
		AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(OtherActor);
		if (Enemy == CombatTarget) {
			UpdateCombatTarget(); // UpdateCombatTarget 함수 안에 MoveToTarget함수가 있고 거기서 MovementStatus == MoveToTarget으로 바꾸어 줌
		}
	}
}

void AMyHeroChar::AttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		//&& HeroMovementStatus != EHeroMovementStatus::EMS_Attacking
		AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(OtherActor);
		if (Enemy) {
			if (Enemy == CombatTarget) {
				// ================작업중==============
				if (AIController) {
					if (DamageTypeClass) {
						UGameplayStatics::ApplyDamage(Enemy, OffensivePower * 1.5, AIController, this, DamageTypeClass);
					}
				}
				// AttackStart();
			}
		}
	}
}

void AMyHeroChar::AttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && HeroMovementStatus == EHeroMovementStatus::EMS_Attacking) {
		AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(OtherActor);
		if (Enemy) {
			if (Enemy == CombatTarget) {
				// AttackEnd();
			}
		}
	}
}

float AMyHeroChar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) {
		/*
		* =======================================================================
		여기는 뭐지.... 애초에도 작성하다가 끝났는데 뭐하는 조건문일까 ?
		==========================================================================
		*/
	}
	if (Health - DamageAmount <= 0.f) {
		Health = 0.f;
		DieStart(DamageCauser);
	}
	else {
		Health -= DamageAmount;
	}
	return DamageAmount;
}

void AMyHeroChar::ReceiveRadialDamage(float DamageReceived, const UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health - DamageReceived <= 0.f) {
		Health = 0.f;
		DieStart(DamageCauser);
	}
	else {
		Health -= DamageReceived;
	}
}

void AMyHeroChar::AttackStart()
{
	if (Super::CanAttack() && CombatTarget != nullptr) {
		Super::AttackStart(); // SetHeroStatus -> Attacking
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance) {
			if (CombatTarget) {
				if (bIsShortDistacne) {
					// 근거리 챔프
					AnimInstance->Montage_Play(CombatMontage, 1.f + AttackSpeed / 10);
					AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
				}
				else {
					// 원거리 챔프 
					AnimInstance->Montage_Play(CombatMontage, 1.f + AttackSpeed / 10); // 0.5로하면 기존 스피드 1/2배
					AnimInstance->Montage_JumpToSection(FName("Attack2"), CombatMontage);
				}
			}
			else {
				SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
				UpdateCombatTarget();
			}
		}
	}
}

void AMyHeroChar::AttackEnd()
{
	// 애니메이션 블루프린트에서 노티파이를 이용하여 호출하는 함수
	Super::AttackEnd(); // SetHeroStatus -> Idle
	if (CombatTarget) {
		if (Super::CanAttack()) {
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AMyHeroChar::AttackStart, AttackTime);
		}
	}
}

void AMyHeroChar::HittedStart()
{
	if (Super::CanHitted()) {
		Super::HittedStart(); // SetHeroStatus -> Hitted
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance) {
			AnimInstance->Montage_Play(CombatMontage, 1.5f);
			AnimInstance->Montage_JumpToSection(FName("Hitted"), CombatMontage);
		}
	}
}

void AMyHeroChar::HittedEnd()
{
	Super::HittedEnd();
	AttackStart();
}

void AMyHeroChar::DieStart(AActor* Causer)
{
	Super::DieStart(Causer);
	if (GetHeroMovementStatus() == EHeroMovementStatus::EMS_Dead) {
		UE_LOG(LogTemp, Warning, TEXT("EMS_Dead change!"));
	}
	for (auto Actor : EnemyArray) {
		AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(Actor);
		if (Enemy) {
			if (Enemy->CombatTarget == this) {
				Enemy->CombatTarget = nullptr;
				Enemy->SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
			}
			if (Enemy->MyHeroArray.Contains(this)) {
				Enemy->MyHeroArray.Remove(this);
			}
		}
	}
}


void AMyHeroChar::DieEnd()
{
	Super::DieEnd();

	for (auto Actor : EnemyArray) {
		AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(Actor);
		if (Enemy) {
			Enemy->UpdateCombatTarget(); // 가까운 적 업데이트 시켜주기
		}
	}
}


void AMyHeroChar::UseSkillStart()
{
}

void AMyHeroChar::UseSkillEnd()
{
}

void AMyHeroChar::GameTimeOver()
{
	Super::GameTimeOver();
}

void AMyHeroChar::UpdateCombatTarget()
{
	if (Super::CanMoveTo()) {
		// 움직일 수 있는 상태이면
		if (EnemyArray.Num() == 0) {
			// Enemy배열이 비어있다면
			return;
		}
		// 제일 가까운 적을 배열의 0번째 인덱스의 값으로 초기화하고
		AEnemyHeroChar* ClosestEnemy = Cast<AEnemyHeroChar>(EnemyArray[0]);
		if (ClosestEnemy) {
			// ClosestEnemy가 존재한다면(즉, 0번째 인덱스 값이 nullptr이 아니라면)
			// ClosestEnemy와의 거리를 MinDIstance로 초기화
			float MinDistance = (ClosestEnemy->GetActorLocation() - GetActorLocation()).Size();

			for (auto Actor : EnemyArray) {
				// 모든 배열을 돌면서
				AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(Actor);
				if (Enemy) {
					// && Enemy->IsAlive()
					// 배열의 해당 Enemy와의 거리를 구한 뒤 
					float DistanceToActor = (Enemy->GetActorLocation() - GetActorLocation()).Size();

					if (MinDistance > DistanceToActor) {
						// Mindistance와 비교한 뒤 더 작으면 이놈으로 초기화
						MinDistance = DistanceToActor;
						ClosestEnemy = Enemy;
					}
				}
			}
			// CombatTarget은 제일 가까운 적으로 설정 
			CombatTarget = ClosestEnemy;
			if (CombatStartCollision->GetScaledSphereRadius() >= MinDistance) {
				// 가장 가까운 적과의 거리가 내 컴뱃 콜리전 범위 이내라면 moveto할 필요 x 
				SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
				AttackStart();
			}
			else {
				// CombatTarget으로 이동! 
				MoveToEnemy(CombatTarget);
			}
		}
	}
}
