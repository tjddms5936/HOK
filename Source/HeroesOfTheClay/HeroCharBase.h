// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeroesOfTheClayGameModeBase.h"
#include "BattleGameModeBase.h"
#include "HeroCharBase.generated.h"


UENUM(BlueprintType)
enum class EHeroMovementStatus : uint8
{
	EMS_Idle			UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DisplayName = "Attacking"),
	EMS_Dead			UMETA(DisplayName = "Dead"),
	EMS_Hitted			UMETA(DisplayName = "Hitted"),

	EMS_MAX				UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class HEROESOFTHECLAY_API AHeroCharBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHeroCharBase();

	// �� ��ȯ�� ���� ������ �ʿ��� Component ���� ���ʿ�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharBase")
	class UCombatComponent* CombatComponent;

	// �ش� Collision ���� ���� ������ ������ �� Ž���� �� ������ �̵���. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase")
	class USphereComponent* MoveToCollision;

	// �ش� Collision ���� ���� ������ ������ ������ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase")
	class USphereComponent* CombatStartCollision;

	// ���̷����� ���� ���Ͽ� ���� ���Ѽ�. ������ ���� ��������, �ִϸ��̼� ��Ÿ�� �̿��ؼ� ������ ���� �ݸ��� on ���Ѽ� ���� �� ���� ������ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase")
	class  USphereComponent* AttackCollision;

	// AI�� MoveTo �ϱ� ���� ���� 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharBase | AI")
	class AAIController* AIController;

	// ���̵� Main�̵� �������� Sound���� ��. �׳� Į�´� ȿ���� �����͸� ��_��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharBase | Sound")
	class USoundCue* HittingSound;

	// ���̵� Main�̵� �������� ����Ʈ �Ͼ�� ��.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | AI")
	class UParticleSystem* HittingParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	FTimerHandle DeathTimer;


	// ĳ���� MovementStatus ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharBase | Movement")
	EHeroMovementStatus HeroMovementStatus;

	// ĳ���� MovementStatus Set,Get Inline���� ����
	FORCEINLINE void SetHeroMovementStatus(EHeroMovementStatus Status) { HeroMovementStatus = Status; }
	FORCEINLINE EHeroMovementStatus GetHeroMovementStatus() { return HeroMovementStatus; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharBase | Combat")
	class UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharBase | Combat")
	class ABattleGameModeBase* GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill System")
	struct FAbilityDetails SkillDataStruct;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// �ִ� ü��. ���߿� ������.. ��� �ִ�ü�� �� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float MaxHealth;
	
	// ���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float Health;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float DefensivePower;

	// ���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float OffensivePower;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float MagicPower;

	// ���ݼӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float AttackSpeed;

	// ���ݰ� ���� ���� �� �ּҽð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float AttackMinTime;

	// ���ݰ� ���� ���� �� ����ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float AttackMaxTime;

	// ������ ���� �ڿ� ����� ���̳�.(���� �ִϸ��̼� �� �ߵ� �� �� ���� �ڿ� ����� ���̳�)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharBase | Info")
	float DeathDelay;

	// �ʵ� ���� �����Ǿ��� �� ��� ° ��ġ�� �ִ��� �˷��ִ� ����. 0~4������ �ִ�. BattilePosition���κ��� ���޹���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	int32 CharIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	bool bIsShortDistacne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	FVector2D HealthBarPositionViewport;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	FVector2D HealthBarSizeInViewport;

	FTimerHandle AttackTimer;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill System")
	FName SkillDataRowName;

	UFUNCTION()
	virtual void MoveToCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void MoveToCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CombatCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void CombatCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SetCombatCollision();

	UFUNCTION()
	virtual void AttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void AttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// MyHero, Enemy�� ���� �ٸ��� ���� + ĳ���� Ŭ������ ���� �ٸ��� ����
	/*UFUNCTION(BlueprintCallable)
	virtual void MoveToEnemy(AEnemyHeroChar* TargetEnemy);*/

	// �⺻ ���� �Լ� : ��Ÿ�� ���� �� ĳ���� Ŭ�������� �޶�� �ϴϱ�.... �߻����� + � Ŭ������ ���������� �ߵ��Ǵ� �˰����� ������ ���� 
	virtual void AttackStart();

	// ���� ������ �� ��� ���� ��ȭ�� �־��� ���� ���� �� ������ ������� ������ ������. ���⼭ ������� ������, ��Ÿ�� ���� ��Ƽ���̸� �̿��Ͽ� �Լ� ȣ��
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	// �±� ������ �� �Լ�
	UFUNCTION(BlueprintCallable)
	virtual void HittedStart();

	// �±� ������ �� ��� ���� ��ȭ�� �־��� �´°� ���� �� ������ ������� ������ ������. ���⼭ �̾𷡴�� ������, ��Ÿ�� ���� ��Ƽ���̸� �̿��Ͽ� �Լ� ȣ��
	UFUNCTION(BlueprintCallable)
	virtual void HittedEnd();

	// �׾��� �� ���� �Լ�. ��Ÿ�� �����̶����. Causer�� ���� ���� ��. �Ǵ� MyHero
	UFUNCTION(BlueprintCallable)
	virtual void DieStart(AActor* Causer);

	// ���� �ִϸ��̼� ���� �� Ÿ�ֿ̹� ��Ƽ���� �߰� �ؼ� -> ĳ���� �� ������� ����� ����. Die���ڸ��� ������� �ִϸ��̼� ���۵� ���ϰ� �ٷ� �����
	UFUNCTION(BlueprintCallable)
	virtual void DieEnd();

	// ��ų ���� ��Ÿ�� ����... �̿ܿ� �� ������ ���� �� : MyHero�� ���� ��ų ���ý�.. Enemy�� �ڵ�����..
	virtual void UseSkillStart();

	// ��������.. ��Ƽ���� �̿��ؼ� ���� �� �� ������ �� 
	virtual void UseSkillEnd();

	// ������ �ֱ� ���� �Լ� �� ��... �̰͵� �������� �����ؾ� �ҵ�. �ֳ���� Die(Causer)�� �� �Ŷ�..
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ReceiveRadialDamage(float DamageReceived, const class UDamageType* DamageType, FVector Origin, const struct FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	// ���� Ÿ�� �����Ǹ� ĳ���� ������, ��ų �� ��ž
	UFUNCTION(BlueprintCallable)
	virtual void GameTimeOver();


	// ���� �� ĳ���� �ʵ� �󿡼� ������� �ϱ� ���� �Լ�
	UFUNCTION()
	void Disappear();

	UFUNCTION(BlueprintCallable)
	bool CanMoveTo();

	UFUNCTION(BlueprintCallable)
	bool CanAttack();

	UFUNCTION(BlueprintCallable)
	bool CanHitted();

	UFUNCTION(BlueprintCallable)
	bool IsAlive();

	UFUNCTION()
	virtual FRotator GetLookAtRotationYaw(FVector Target);
};
