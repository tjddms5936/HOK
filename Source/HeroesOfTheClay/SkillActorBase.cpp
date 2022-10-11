// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActorBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "MyHeroChar.h"
#include "EnemyHeroChar.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASkillActorBase::ASkillActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	SkillSpawnParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SkillParticle"));
	SkillSpawnParticle->SetupAttachment(GetRootComponent());


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1200.f;
	ProjectileMovement->MaxSpeed = 1200.f;
	ProjectileMovement->ProjectileGravityScale = 0.f; // 0�̻��̸� ���ٰ� ������ 

	HitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(GetRootComponent());


	DestroyDelay = 3.f;

	SkillDamage = 10.f;
	MinDmg = 10.f;
	MaxDmg = 10.f;

	IsFromMyHero = true;

}

// Called when the game starts or when spawned
void ASkillActorBase::BeginPlay()
{
	Super::BeginPlay();

	// ���Ϳ��� ���� �ʾƵ� 3�ʵڸ� ������ ���������� ����.
	GetWorldTimerManager().SetTimer(DestroyTime, this, &ASkillActorBase::Disappear, DestroyDelay);

	// HitCollision �ݸ��� ����. Enemy���� �� Ignore. 
	HitCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HitCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkillActorBase::HitOnOverlapBegin);
	HitCollision->OnComponentEndOverlap.AddDynamic(this, &ASkillActorBase::HitOnOverlapEnd);
}

// Called every frame
void ASkillActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillActorBase::Disappear()
{
	Destroy();
}

void  ASkillActorBase::HitOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		if (IsFromMyHero) {
			// �� ĳ���Ͱ� ���� ��ų�̶��
			AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(OtherActor);
			if (Enemy) {
				if (Enemy->CanHitted()) {
					Enemy->SetHeroMovementStatus(EHeroMovementStatus::EMS_Hitted);
					if (DamageTypeClass) {
						// ������ ������ ������
						UGameplayStatics::ApplyDamage(Enemy, SkillDamageRand(), SkillInstigator, this, DamageTypeClass);
						Enemy->HittedStart(); // �´� �ִϸ��̼� ����
					}
					if (HittingParticle) {
						// ��ų ���� ����Ʈ �ߵ�
						if (SkillSpawnParticle) {
							// SkillSpawnParticle->SetVisibility(false);
						}
						FVector HitLocation = Enemy->GetActorLocation();
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HittingParticle, HitLocation);
					}
					if(HittingSound){
						// ��ų ���� ���� �ߵ�
						UGameplayStatics::PlaySound2D(this, HittingSound);
					}
					// Ÿ�̸ӷ� ��ų Actor �ı�
					GetWorldTimerManager().SetTimer(DestroyTime, this, &ASkillActorBase::Disappear, DestroyDelay);
				}
			}
		}
		else {
			// �� ĳ���Ͱ� ���� ��ų�̶��
			AMyHeroChar* MyHero = Cast<AMyHeroChar>(OtherActor);
			if (MyHero) {
				if (MyHero->CanHitted()) {
					MyHero->SetHeroMovementStatus(EHeroMovementStatus::EMS_Hitted);
					if (DamageTypeClass) {
						// ������ ������ ������
						UGameplayStatics::ApplyDamage(MyHero, SkillDamageRand(), SkillInstigator, this, DamageTypeClass);
						MyHero->HittedStart(); // �´� �ִϸ��̼� ����
						UE_LOG(LogTemp, Warning, TEXT("OK DamageTypeClass or SkilllInstigator"));
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("No DamageTypeClass or SkilllInstigator"));
					}
					if (HittingParticle) {
						// ��ų ���� ����Ʈ �ߵ�
						if (SkillSpawnParticle) {
							// SkillSpawnParticle->SetVisibility(false);
						}
						FVector HitLocation = MyHero->GetActorLocation();
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HittingParticle, HitLocation);
					}
					if (HittingSound) {
						// ��ų ���� ���� �ߵ�
						UGameplayStatics::PlaySound2D(this, HittingSound);
					}
					// Ÿ�̸ӷ� ��ų Actor �ı�
					GetWorldTimerManager().SetTimer(DestroyTime, this, &ASkillActorBase::Disappear, DestroyDelay);
				}
			}
		}
	}

}

void  ASkillActorBase::HitOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor) {
		if (IsFromMyHero) {
			// �� ĳ���Ͱ� ���� ��ų�̶��
			AEnemyHeroChar* Enemy = Cast<AEnemyHeroChar>(OtherActor);
			if (Enemy) {

			}
		}
		else {
			// �� ĳ���Ͱ� ���� ��ų�̶��
			AMyHeroChar* MyHero = Cast<AMyHeroChar>(OtherActor);
			if (MyHero) {

			}
		}
	}
}

float ASkillActorBase::SkillDamageRand()
{
	float RandomDamage = FMath::FRandRange(MinDmg, MaxDmg);
	return RandomDamage;
}