// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroCharBase.h"
#include "CombatComponent.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "BattleGameModeBase.h"
#include "BattleController.h"
// Sets default values
AHeroCharBase::AHeroCharBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	MoveToCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MoveToCollision"));
	MoveToCollision->SetupAttachment(GetRootComponent());
	MoveToCollision->InitSphereRadius(200.f);
	MoveToCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	CombatStartCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CombatStartCollision"));
	CombatStartCollision->SetupAttachment(GetRootComponent());
	CombatStartCollision->InitSphereRadius(60.f);

	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetMesh(), FName("AttackSocket1"));

	SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);

	// 최대 체력
	MaxHealth = 1000.f;

	// 현재 체력
	Health = 1000.f;

	// 방어력
	DefensivePower = 10.f;

	// 공격력
	OffensivePower = 10.f;

	// 마력
	MagicPower = 10.f;

	// 공격속도
	AttackSpeed = 1.f;

	// 죽음 후 몇초 뒤 사라질 것인지
	DeathDelay = 1.5f;

	AttackMinTime = 0.6f - (AttackSpeed / 30);

	AttackMaxTime = 3.f - (AttackSpeed / 5) + AttackMinTime;

	CharIndex = 1;

	bIsShortDistacne = true;

	HealthBarSizeInViewport = FVector2D(150.f, 17.f);
}

// Called when the game starts or when spawned
void AHeroCharBase::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());
	GameMode = Cast<ABattleGameModeBase>(GetWorld()->GetAuthGameMode());

	MoveToCollision->OnComponentBeginOverlap.AddDynamic(this, &AHeroCharBase::MoveToCollisionBeginOverlap);
	MoveToCollision->OnComponentEndOverlap.AddDynamic(this, &AHeroCharBase::MoveToCollisionEndOverlap);


	CombatStartCollision->OnComponentBeginOverlap.AddDynamic(this, &AHeroCharBase::CombatCollisionBeginOverlap);
	CombatStartCollision->OnComponentEndOverlap.AddDynamic(this, &AHeroCharBase::CombatCollisionEndOverlap);
	if (!bIsShortDistacne) {
		// 원거리 캐릭터라면 
		SetCombatCollision(); // 재귀함수 이용해서 600.f까지 반지름 넓혀주기
	}



	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AHeroCharBase::AttackCollisionBeginOverlap);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &AHeroCharBase::AttackCollisionEndOverlap);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 기본적으로 NoCollision하고 몽타주이용해서 On시킴. 지금은 Test용으로 On
	AttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	
}

// Called every frame
void AHeroCharBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttackMinTime = 0.6f - (AttackSpeed / 30);

	AttackMaxTime = 3.f - (AttackSpeed / 5) + AttackMinTime;
}

// Called to bind functionality to input
void AHeroCharBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHeroCharBase::MoveToCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AHeroCharBase::MoveToCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AHeroCharBase::CombatCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AHeroCharBase::CombatCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AHeroCharBase::SetCombatCollision()
{
	if (CombatStartCollision->GetScaledSphereRadius() >= 600.f){
		return;
	}
	else {
		CombatStartCollision->SetSphereRadius(CombatStartCollision->GetScaledSphereRadius() + 1);
		SetCombatCollision();
	}
}

void AHeroCharBase::AttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AHeroCharBase::AttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


void AHeroCharBase::AttackStart()
{
	SetHeroMovementStatus(EHeroMovementStatus::EMS_Attacking);
}

void AHeroCharBase::AttackEnd()
{
	SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
}

void AHeroCharBase::HittedStart()
{
	SetHeroMovementStatus(EHeroMovementStatus::EMS_Hitted);
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AHeroCharBase::HittedEnd()
{
	SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AHeroCharBase::DieStart(AActor* Causer)
{
	SetHeroMovementStatus(EHeroMovementStatus::EMS_Dead);
	
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance) {
		AnimInstance->Montage_Play(CombatMontage, 2.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
	
}

void AHeroCharBase::DieEnd() {
	// 노티파이 관련해서 캐릭터 TimeHandler 이용해서 없앤다던가 ㄱㄱ.. 
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AHeroCharBase::Disappear, DeathDelay);
}

void AHeroCharBase::Disappear() {
	Destroy();
}

void AHeroCharBase::UseSkillStart()
{
	SetHeroMovementStatus(EHeroMovementStatus::EMS_Attacking);
}

void AHeroCharBase::UseSkillEnd()
{
	SetHeroMovementStatus(EHeroMovementStatus::EMS_Idle);
}

float AHeroCharBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return DamageAmount;
}

void AHeroCharBase::ReceiveRadialDamage(float DamageReceived, const UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
}

void AHeroCharBase::GameTimeOver()
{
	if (GetHeroMovementStatus() == EHeroMovementStatus::EMS_Dead) {
		// 죽자마자 게임오버되면? 바로 캐릭 없애주자 안그러면 체력 0인 상태로 idle 애니메이션 취해버림
		Destroy();
	}
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Play(CombatMontage, 2.0f);
		AnimInstance->Montage_JumpToSection(FName("GameTimeOver"), CombatMontage);
	}
}

bool AHeroCharBase::CanMoveTo()
{
	return (GetHeroMovementStatus() != EHeroMovementStatus::EMS_Dead && 
		GetHeroMovementStatus() != EHeroMovementStatus::EMS_Attacking);
}
bool AHeroCharBase::CanAttack()
{
	return (HeroMovementStatus != EHeroMovementStatus::EMS_Attacking &&
		HeroMovementStatus != EHeroMovementStatus::EMS_Dead);
}
bool AHeroCharBase::CanHitted()
{
	return GetHeroMovementStatus() != EHeroMovementStatus::EMS_Dead;
}

bool AHeroCharBase::IsAlive()
{
	// 죽은 상태가 아니라면 true 반환
	return GetHeroMovementStatus() != EHeroMovementStatus::EMS_Dead;
}


FRotator AHeroCharBase::GetLookAtRotationYaw(FVector Target)
{
	return FRotator();
}


