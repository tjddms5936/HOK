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
	// Target�� ���ƺ��� ����
	if (CombatTarget && GetHeroMovementStatus() != EHeroMovementStatus::EMS_Dead) {
		// �� �ΰ��� ������ �����ϸ� ���� ���� : ���� ���� �ٶ󺸴� ���� 
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, 15.f);

		SetActorRotation(InterpRotation);
	}
}

void AMyHeroChar::MoveToEnemy(AEnemyHeroChar* TargetEnemy)
{
	SetHeroMovementStatus(EHeroMovementStatus::EMS_MoveToTarget);
	if (AIController) {
		// AI�� Move To �ϴ� �۾��� ���������� �����ϴ� ���ذ� �Ȱ��� un4 ���� ������ �� 

		// �̵��� ���� ���� ����
		FAIMoveRequest MoveRequest;
		// �Ʒ� �ΰ��� ������ �������� Default
		MoveRequest.SetGoalActor(TargetEnemy);
		if (bIsShortDistacne == true) {
			// �ٰŸ� ĳ���Ͷ��
			MoveRequest.SetAcceptanceRadius(70.0f); // ĳ���� 10unit�Ÿ��� �Ǹ� �̵� �Ϸ�� ����
		}
		else {
			// ���Ÿ� ĳ���Ͷ��
			MoveRequest.SetAcceptanceRadius(500.0f); // ĳ���� 70unit�Ÿ��� �Ǹ� �̵� �Ϸ�� ����
		}
		

		// �Ҵ�� ��η� ä���� ������ ��� �Ű�����
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
	// ���� �ʿ䰡 ������? ����.. �� ������ ! 
	
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
			UpdateCombatTarget(); // UpdateCombatTarget �Լ� �ȿ� MoveToTarget�Լ��� �ְ� �ű⼭ MovementStatus == MoveToTarget���� �ٲپ� ��
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
				// ================�۾���==============
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
		����� ����.... ���ʿ��� �ۼ��ϴٰ� �����µ� ���ϴ� ���ǹ��ϱ� ?
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
					// �ٰŸ� è��
					AnimInstance->Montage_Play(CombatMontage, 1.f + AttackSpeed / 10);
					AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
				}
				else {
					// ���Ÿ� è�� 
					AnimInstance->Montage_Play(CombatMontage, 1.f + AttackSpeed / 10); // 0.5���ϸ� ���� ���ǵ� 1/2��
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
	// �ִϸ��̼� �������Ʈ���� ��Ƽ���̸� �̿��Ͽ� ȣ���ϴ� �Լ�
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
			Enemy->UpdateCombatTarget(); // ����� �� ������Ʈ �����ֱ�
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
		// ������ �� �ִ� �����̸�
		if (EnemyArray.Num() == 0) {
			// Enemy�迭�� ����ִٸ�
			return;
		}
		// ���� ����� ���� �迭�� 0��° �ε����� ������ �ʱ�ȭ�ϰ�
		AEnemyHeroChar* ClosestEnemy = Cast<AEnemyHeroChar>(EnemyArray[0]);
		if (ClosestEnemy) {
			// ClosestEnemy�� �����Ѵٸ�(��, 0��° �ε��� ���� nullptr�� �ƴ϶��)
			// ClosestEnemy���� �Ÿ��� MinDIstance�� �ʱ�ȭ
			float MinDistance = (ClosestEnemy->GetActorLocation() - GetActorLocation()).Size();

			for (auto Actor : EnemyArray) {
				// ��� �迭�� ���鼭
				AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(Actor);
				if (Enemy) {
					// && Enemy->IsAlive()
					// �迭�� �ش� Enemy���� �Ÿ��� ���� �� 
					float DistanceToActor = (Enemy->GetActorLocation() - GetActorLocation()).Size();

					if (MinDistance > DistanceToActor) {
						// Mindistance�� ���� �� �� ������ �̳����� �ʱ�ȭ
						MinDistance = DistanceToActor;
						ClosestEnemy = Enemy;
					}
				}
			}
			// CombatTarget�� ���� ����� ������ ���� 
			CombatTarget = ClosestEnemy;
			if (CombatStartCollision->GetScaledSphereRadius() >= MinDistance) {
				// ���� ����� ������ �Ÿ��� �� �Ĺ� �ݸ��� ���� �̳���� moveto�� �ʿ� x 
				SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
				AttackStart();
			}
			else {
				// CombatTarget���� �̵�! 
				MoveToEnemy(CombatTarget);
			}
		}
	}
}
