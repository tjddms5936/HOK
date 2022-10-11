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

	// 턴 전환제 게임 연습에 필요한 Component 이후 불필요시 삭제
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharBase")
	class UCombatComponent* CombatComponent;

	// 해당 Collision 범위 내에 있으면 적절한 적 탐색후 그 적한테 이동함. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase")
	class USphereComponent* MoveToCollision;

	// 해당 Collision 범위 내에 있으면 공격을 시작함 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase")
	class USphereComponent* CombatStartCollision;

	// 스켈레톤의 지정 소켓에 장착 시켜서. 지금은 구현 안했지만, 애니메이션 몽타주 이용해서 공격할 때만 콜리전 on 시켜서 공격 시 상대방 데미지 입히기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase")
	class  USphereComponent* AttackCollision;

	// AI로 MoveTo 하기 위한 변수 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharBase | AI")
	class AAIController* AIController;

	// 적이든 Main이든 때릴때만 Sound나면 됨. 그냥 칼맞는 효과음 같은것만 ㅇ_ㅇ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharBase | Sound")
	class USoundCue* HittingSound;

	// 적이든 Main이든 때릴때만 이펙트 일어나면 됨.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | AI")
	class UParticleSystem* HittingParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	FTimerHandle DeathTimer;


	// 캐릭터 MovementStatus 열거형 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharBase | Movement")
	EHeroMovementStatus HeroMovementStatus;

	// 캐릭터 MovementStatus Set,Get Inline으로 구성
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
	// 최대 체력. 나중에 버프나.. 등등 최대체력 쓸 일 있음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float MaxHealth;
	
	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float Health;

	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float DefensivePower;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float OffensivePower;

	// 마력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float MagicPower;

	// 공격속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float AttackSpeed;

	// 공격과 공격 사이 텀 최소시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float AttackMinTime;

	// 공격과 공격 사이 텀 최장시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharBase | Info")
	float AttackMaxTime;

	// 죽음시 몇초 뒤에 사라질 것이냐.(죽음 애니메이션 다 발동 한 뒤 몇초 뒤에 사라질 것이냐)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharBase | Info")
	float DeathDelay;

	// 필드 위에 스폰되었을 때 몇번 째 위치에 있는지 알려주는 변수. 0~4번까지 있다. BattilePosition으로부터 전달받음
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
	// MyHero, Enemy에 따라 다르게 설정 + 캐릭터 클래스에 따라 다르게 설정
	/*UFUNCTION(BlueprintCallable)
	virtual void MoveToEnemy(AEnemyHeroChar* TargetEnemy);*/

	// 기본 공격 함수 : 몽타주 적용 등 캐릭터 클래스마다 달라야 하니까.... 추상으로 + 어떤 클래스든 공통적으로 발동되는 알고리즘이 있으면 적용 
	virtual void AttackStart();

	// 공격 시작할 때 어떠한 변수 변화가 있었고 공격 끝날 때 변수를 원래대로 돌리고 싶으면. 여기서 원래대로 돌리고, 몽타주 섹션 노티파이를 이용하여 함수 호출
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	// 맞기 시작할 때 함수
	UFUNCTION(BlueprintCallable)
	virtual void HittedStart();

	// 맞기 시작할 때 어떠한 변수 변화가 있었고 맞는거 끝날 때 변수를 원래대로 돌리고 싶으면. 여기서 ㅜ언래대로 돌리고, 몽타주 섹션 노티파이를 이용하여 함수 호출
	UFUNCTION(BlueprintCallable)
	virtual void HittedEnd();

	// 죽었을 때 관련 함수. 몽타주 적용이라던가. Causer는 나를 죽인 적. 또는 MyHero
	UFUNCTION(BlueprintCallable)
	virtual void DieStart(AActor* Causer);

	// 죽음 애니메이션 끝날 때 타이밍에 노티파이 추가 해서 -> 캐릭터 뭐 사라지게 라던가 조정. Die하자마자 사라지면 애니메이션 동작도 안하고 바로 사라짐
	UFUNCTION(BlueprintCallable)
	virtual void DieEnd();

	// 스킬 사용시 몽타주 설정... 이외에 뭐 있으면 설정 ㄱ : MyHero는 내가 스킬 선택시.. Enemy는 자동으로..
	virtual void UseSkillStart();

	// 마찬가지.. 노티파이 이용해서 뭔가 할 거 있으면 해 
	virtual void UseSkillEnd();

	// 데미지 주기 관련 함수 둘 다... 이것도 각각에서 설정해야 할듯. 왜냐햐면 Die(Causer)를 쓸 거라..
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ReceiveRadialDamage(float DamageReceived, const class UDamageType* DamageType, FVector Origin, const struct FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	// 게임 타임 오버되면 캐릭터 움직임, 스킬 올 스탑
	UFUNCTION(BlueprintCallable)
	virtual void GameTimeOver();


	// 죽음 시 캐릭터 필드 상에서 사라지게 하기 위한 함수
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
