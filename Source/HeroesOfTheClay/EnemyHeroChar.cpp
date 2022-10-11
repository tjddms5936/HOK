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
	// Target�� ���ƺ��� ����
	if (CombatTarget && GetHeroMovementStatus() != EHeroMovementStatus::EMS_Dead) {
		// �� �ΰ��� ������ �����ϸ� ���� ���� : ���� ���� �ٶ󺸴� ���� 
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, 15.f);

		SetActorRotation(InterpRotation);
	}
}

void AEnemyHeroChar::MoveToMyHero(AMyHeroChar* TargetEnemy)
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
			MoveRequest.SetAcceptanceRadius(30.0f); // ĳ���� 10unit�Ÿ��� �Ǹ� �̵� �Ϸ�� ����
		}
		else {
			// ���Ÿ� ĳ���Ͷ��
			MoveRequest.SetAcceptanceRadius(500.0f); // ĳ���� 70unit�Ÿ��� �Ǹ� �̵� �Ϸ�� ����
		}


		// �Ҵ�� ��η� ä���� ������ ��� �Ű�����
		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		// TArray<FNavPathPoint>  PathPoint = NavPath->GetPathPoints();
		// == auto PathPoint = NavPath->GetPathPoints();

		/**
		* �̵���� Ȯ�ο� �ڵ�
		* �̵���� ? AI�� ���� ����ö� � ��η� ���°�? DrawDebug�ؼ� ���� ���� ��η� ��
		*
		for (FNavPathPoint Point : PathPoint) {


			// ���⼭ Point�� PathPoint�迭�� �� �����.
			// Point�� ���������� ����ü�� �����Ǿ� �ְ� �� �� �츮�� FVector Location�� �ʿ�

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
	// ���� �ʿ䰡 ������? ����.. �� ������ ! 
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
			UpdateCombatTarget(); // UpdateCombatTarget �Լ� �ȿ� MoveToTarget�Լ��� �ְ� �ű⼭ MovementStatus == MoveToTarget���� �ٲپ� ��
		}
	}
}

void AEnemyHeroChar::AttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AMyHeroChar* MyHero = Cast<AMyHeroChar>(OtherActor);
		if (MyHero) {
			if (MyHero == CombatTarget) {
				// ================�۾���==============
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
					// �ٰŸ� è��
					AnimInstance->Montage_Play(CombatMontage, 1.f + (AttackSpeed / 10));
					AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
				}
				else {
					// ���Ÿ� è�� 
					AnimInstance->Montage_Play(CombatMontage, 1.f + (AttackSpeed / 10)); // 0.5���ϸ� ���� ���ǵ� 1/2��
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
	// �ִϸ��̼� �������Ʈ���� ��Ƽ���̸� �̿��Ͽ� ȣ���ϴ� �Լ�
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
			MyHero->UpdateCombatTarget(); // ����� �� ������Ʈ �����ֱ�
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
						// =============== �۾� �� ====================
					}
				}
			}
			CombatTarget = ClosestMyHero;
			if (CombatStartCollision->GetScaledSphereRadius() >= MinDistance) {
				// ���� ����� ������ �Ÿ��� �� �Ĺ� �ݸ��� ���� �̳���� moveto�� �ʿ� x 
				SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
				AttackStart();
			}
			else {
				// CombatTarget���� �̵�! 
				MoveToMyHero(CombatTarget);
			}
		}
	}
}


