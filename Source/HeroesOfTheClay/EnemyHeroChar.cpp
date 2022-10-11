// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHeroChar.h"
#include "MyHeroChar.h"
#include "AIController.h"
#include "Components/SphereComponent.h"
#include "BattleGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
AEnemyHeroChar::AEnemyHeroChar()
{
	// CombatStartCollision->InitSphereRadius(15.f);

	SpawnSkillTimes = 1;
}

void AEnemyHeroChar::BeginPlay()
{
	Super::BeginPlay();
	if (!AIController) {
		AIController = Cast<AAIController>(GetController());
	}
}

void AEnemyHeroChar::Tick(float DeltaTime)
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

void AEnemyHeroChar::MoveToMyHero(AMyHeroChar* TargetEnemy)
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
			MoveRequest.SetAcceptanceRadius(30.0f); // 캐릭과 10unit거리가 되면 이동 완료로 간주
		}
		else {
			// 원거리 캐릭터라면
			MoveRequest.SetAcceptanceRadius(500.0f); // 캐릭과 70unit거리가 되면 이동 완료로 간주
		}


		// 할당된 경로로 채워진 선택적 출력 매개변수
		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		// TArray<FNavPathPoint>  PathPoint = NavPath->GetPathPoints();
		// == auto PathPoint = NavPath->GetPathPoints();

		/**
		* 이동경로 확인용 코드
		* 이동경로 ? AI가 나를 따라올때 어떤 경로로 오는가? DrawDebug해서 보면 직선 경로로 옴
		*
		for (FNavPathPoint Point : PathPoint) {


			// 여기서 Point는 PathPoint배열의 각 요소임.
			// Point는 내부적으로 구조체로 구성되어 있고 그 중 우리는 FVector Location이 필요

			FVector Location = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, 2.5f);
		}
		*/
	}
}




FRotator AEnemyHeroChar::GetLookAtRotationYaw(FVector Target)
{
	// FindLookAtRotation : Find a rotation for an object at Start location to point at Target location.
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AEnemyHeroChar::MoveToCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AMyHeroChar* MyHero = Cast<AMyHeroChar>(OtherActor);
		if (MyHero) {
			MyHeroArray.Add(MyHero);
		}
	}
}

void AEnemyHeroChar::MoveToCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 멈출 필요가 있을까? ㅇㅇ.. 다 죽으면 ! 
}

void AEnemyHeroChar::CombatCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		// UE_LOG(LogTemp, Warning, TEXT("StopMovement!! From Enemy"));
		AMyHeroChar* MyHero = Cast<AMyHeroChar>(OtherActor);
		if (MyHero == CombatTarget) {
			SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
			if (AIController) {
				// AIController->StopMovement();
				// UE_LOG(LogTemp, Warning, TEXT("StopMovement!! From Enemy"));
			}
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyHeroChar::AttackStart, AttackTime);
		}
	}
}

void AEnemyHeroChar::CombatCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor) {
		AMyHeroChar* MyHero = Cast<AMyHeroChar>(OtherActor);
		if (MyHero == CombatTarget) {
			UpdateCombatTarget(); // UpdateCombatTarget 함수 안에 MoveToTarget함수가 있고 거기서 MovementStatus == MoveToTarget으로 바꾸어 줌
		}
	}
}

void AEnemyHeroChar::AttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AMyHeroChar* MyHero = Cast<AMyHeroChar>(OtherActor);
		if (MyHero) {
			if (MyHero == CombatTarget) {
				// ================작업중==============
				if (AIController) {
					if (DamageTypeClass) {
						UGameplayStatics::ApplyDamage(MyHero, OffensivePower * 1.5, AIController, this, DamageTypeClass);
					}
				}
			}
		}
	}
}

void AEnemyHeroChar::AttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && HeroMovementStatus == EHeroMovementStatus::EMS_Attacking) {
		if (Super::CanAttack()) {
			AMyHeroChar* MyHero = Cast<AMyHeroChar>(OtherActor);
			if (MyHero) {
				if (MyHero == CombatTarget) {
				}
			}
		}
	}
}

float AEnemyHeroChar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void AEnemyHeroChar::ReceiveRadialDamage(float DamageReceived, const UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health - DamageReceived <= 0.f) {
		Health = 0.f;
		DieStart(DamageCauser);
	}
	else {
		Health -= DamageReceived;
	}
}

void AEnemyHeroChar::AttackStart()
{
	if (Super::CanAttack() && CombatTarget != nullptr) {
		Super::AttackStart(); // SetHeroStatus -> Attacking
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance) {
			if (CombatTarget) {
				if (bIsShortDistacne) {
					// 근거리 챔프
					AnimInstance->Montage_Play(CombatMontage, 1.f + (AttackSpeed / 10));
					AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
				}
				else {
					// 원거리 챔프 
					AnimInstance->Montage_Play(CombatMontage, 1.f + (AttackSpeed / 10)); // 0.5로하면 기존 스피드 1/2배
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

void AEnemyHeroChar::AttackEnd()
{
	// 애니메이션 블루프린트에서 노티파이를 이용하여 호출하는 함수
	Super::AttackEnd(); // SetHeroStatus -> Idle
	if (CombatTarget) {
		if (Super::CanAttack()) {
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyHeroChar::AttackStart, AttackTime);
		}
	}
}

void AEnemyHeroChar::HittedStart()
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

void AEnemyHeroChar::HittedEnd()
{
	Super::HittedEnd();
	AttackStart();
}

void AEnemyHeroChar::DieStart(AActor* Causer)
{
	Super::DieStart(Causer);
	if (GetHeroMovementStatus() == EHeroMovementStatus::EMS_Dead) {
		UE_LOG(LogTemp, Warning, TEXT("EMS_Dead change!"));
	}
	for (auto Actor : MyHeroArray) {
		AMyHeroChar* MyHero = Cast<AMyHeroChar>(Actor);
		if (MyHero) {
			if (MyHero->CombatTarget == this) {
				MyHero->CombatTarget = nullptr;
				MyHero->SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
			}
			if (MyHero->EnemyArray.Contains(this)) {
				MyHero->EnemyArray.Remove(this);
			}
		}
	}
}

void AEnemyHeroChar::DieEnd()
{
	Super::DieEnd();
	for (auto Actor : MyHeroArray) {
		AMyHeroChar* MyHero = Cast<AMyHeroChar>(Actor);
		if (MyHero) {
			MyHero->UpdateCombatTarget(); // 가까운 적 업데이트 시켜주기
		}
	}
}


void AEnemyHeroChar::UseSkillStart()
{
}

void AEnemyHeroChar::UseSkillEnd()
{
}

void AEnemyHeroChar::GameTimeOver()
{
	Super::GameTimeOver();
}

void AEnemyHeroChar::UpdateCombatTarget()
{
	if (GetHeroMovementStatus() == EHeroMovementStatus::EMS_Attacking && !CombatTarget) {
		SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
		UpdateCombatTarget();
	}
	if (Super::CanMoveTo()) {
		if (MyHeroArray.Num() == 0) {
			return;
		}
		AMyHeroChar* ClosestMyHero = Cast<AMyHeroChar>(MyHeroArray[0]);
		if (ClosestMyHero) {
			float MinDistance = (ClosestMyHero->GetActorLocation() - GetActorLocation()).Size();

			for (auto Actor : MyHeroArray) {
				AMyHeroChar* MyHero = Cast<AMyHeroChar>(Actor);
				if (MyHero) {
					//  && MyHero->IsAlive()
					float DistanceToActor = (MyHero->GetActorLocation() - GetActorLocation()).Size();
					if (MinDistance > DistanceToActor) {
						MinDistance = DistanceToActor;
						ClosestMyHero = MyHero;
						// =============== 작업 중 ====================
					}
				}
			}
			CombatTarget = ClosestMyHero;
			if (CombatStartCollision->GetScaledSphereRadius() >= MinDistance) {
				// 가장 가까운 적과의 거리가 내 컴뱃 콜리전 범위 이내라면 moveto할 필요 x 
				SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
				AttackStart();
			}
			else {
				// CombatTarget으로 이동! 
				MoveToMyHero(CombatTarget);
			}
		}
	}
}


