// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleGameModeBase.h"
#include "SkillActorBase.generated.h"

UCLASS()
class HEROESOFTHECLAY_API ASkillActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillActorBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Details")
	struct FAbilityDetails SkillDetails;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Scene;

	// ��ų ���� ����Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill | Particle")
	class UParticleSystemComponent* SkillSpawnParticle;

	// ������ �¾��� �� ����Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill | Particle")
	class UParticleSystem* HittingParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Combat")
	class UBoxComponent* HitCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Combat")
	AController* SkillInstigator;

	FORCEINLINE void SetInstigator(AController* Inst) { SkillInstigator = Inst; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill | Sound")
	class USoundCue* SkillSpawnSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill | Sound")
	USoundCue* HittingSound;

	// �Լ� �Ű����� ��. ������ ���ʵڿ� ��������� float�� protected�� ����
	FTimerHandle DestroyTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Details")
	class AHeroCharBase* OwnedChar;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Combat")
	float DestroyDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Combat")
	float SkillDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Combat")
	float MinDmg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Combat")
	float MaxDmg;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Disappear();

	UFUNCTION()
	virtual void  HitOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void  HitOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	float SkillDamageRand();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Combat")
	bool IsFromMyHero;
};
