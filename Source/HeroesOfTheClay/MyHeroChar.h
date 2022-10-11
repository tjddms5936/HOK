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
	// ====================================== �̰� ���� �۾� �� ===============================================
	

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

	// �⺻ ���� �Լ� : ��Ÿ�� ���� �� ĳ���� Ŭ�������� �޶�� �ϴϱ�.... �߻����� + � Ŭ������ ���������� �ߵ��Ǵ� �˰����� ������ ���� 
	virtual void AttackStart();

	// ���� ������ �� ��� ���� ��ȭ�� �־��� ���� ���� �� ������ ������� ������ ������. ���⼭ ������� ������, ��Ÿ�� ���� ��Ƽ���̸� �̿��Ͽ� �Լ� ȣ��
	virtual void AttackEnd();

	// �±� ������ �� �Լ�
	virtual void HittedStart();

	// �±� ������ �� ��� ���� ��ȭ�� �־��� �´°� ���� �� ������ ������� ������ ������. ���⼭ �̾𷡴�� ������, ��Ÿ�� ���� ��Ƽ���̸� �̿��Ͽ� �Լ� ȣ��
	virtual void HittedEnd();

	// �׾��� �� ���� �Լ�. ��Ÿ�� �����̶����. Causer�� ���� ���� ��. �Ǵ� MyHero
	virtual void DieStart(AActor* Causer);

	virtual void DieEnd();

	// ��ų ���� ��Ÿ�� ����... �̿ܿ� �� ������ ���� �� : MyHero�� ���� ��ų ���ý�.. Enemy�� �ڵ�����..
	virtual void UseSkillStart();

	// ��������.. ��Ƽ���� �̿��ؼ� ���� �� �� ������ �� 
	virtual void UseSkillEnd();

	virtual void GameTimeOver();

	UFUNCTION(BlueprintCallable)
	void UpdateCombatTarget();


	virtual FRotator GetLookAtRotationYaw(FVector Target) override;


	// ============================================= �� �ý��� ���� ================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	bool IsMyDeckSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	bool IsJustSpawnedForDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	int32 PositionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	int32 ActorIndexFromDeck;
};
