// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeroCharBase.h"
#include "MyHeroChar.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFTHECLAY_API AMyHeroChar : public AHeroCharBase
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMyHeroChar();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ai System")
	class AEnemyHeroChar* CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ai System")
	TSubclassOf<AEnemyHeroChar> EnemyFilter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ai System")
	TArray<AEnemyHeroChar*> EnemyArray;
	// ====================================== 이거 관련 작업 중 ===============================================
	

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);



public:
	UFUNCTION(BlueprintCallable)
	void MoveToEnemy(AEnemyHeroChar* TargetEnemy);

	virtual void MoveToCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void MoveToCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void CombatCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void CombatCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void AttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void AttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ReceiveRadialDamage(float DamageReceived, const class UDamageType* DamageType, FVector Origin, const struct FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	// 기본 공격 함수 : 몽타주 적용 등 캐릭터 클래스마다 달라야 하니까.... 추상으로 + 어떤 클래스든 공통적으로 발동되는 알고리즘이 있으면 적용 
	virtual void AttackStart();

	// 공격 시작할 때 어떠한 변수 변화가 있었고 공격 끝날 때 변수를 원래대로 돌리고 싶으면. 여기서 원래대로 돌리고, 몽타주 섹션 노티파이를 이용하여 함수 호출
	virtual void AttackEnd();

	// 맞기 시작할 때 함수
	virtual void HittedStart();

	// 맞기 시작할 때 어떠한 변수 변화가 있었고 맞는거 끝날 때 변수를 원래대로 돌리고 싶으면. 여기서 ㅜ언래대로 돌리고, 몽타주 섹션 노티파이를 이용하여 함수 호출
	virtual void HittedEnd();

	// 죽었을 때 관련 함수. 몽타주 적용이라던가. Causer는 나를 죽인 적. 또는 MyHero
	virtual void DieStart(AActor* Causer);

	virtual void DieEnd();

	// 스킬 사용시 몽타주 설정... 이외에 뭐 있으면 설정 ㄱ : MyHero는 내가 스킬 선택시.. Enemy는 자동으로..
	virtual void UseSkillStart();

	// 마찬가지.. 노티파이 이용해서 뭔가 할 거 있으면 해 
	virtual void UseSkillEnd();

	virtual void GameTimeOver();

	UFUNCTION(BlueprintCallable)
	void UpdateCombatTarget();


	virtual FRotator GetLookAtRotationYaw(FVector Target) override;


	// ============================================= 덱 시스템 관련 ================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	bool IsMyDeckSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	bool IsJustSpawnedForDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	int32 PositionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	int32 ActorIndexFromDeck;
};
