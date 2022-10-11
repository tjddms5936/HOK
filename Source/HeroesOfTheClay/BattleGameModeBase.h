// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "BattleGameModeBase.generated.h"


/**
 * 
 */

USTRUCT(BlueprintType)
struct FStatus : public FTableRowBase
{
	GENERATED_BODY()
public:
	FStatus() {
		HOK_Coin = 100;
		UserID = FName("HOK_Blue");
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 HOK_Coin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName UserID;
};

USTRUCT(BlueprintType)
struct FQuestStatus : public FTableRowBase
{
	GENERATED_BODY()
public:
	FQuestStatus() {
		QuestName = FText::FromString("Defaults QuestName");
		QuestContent = FText::FromString("Default QuestContent");
		Compensation = FText::FromString("Default Compensation");
		Compensation_Value = 0;
		EventDue = FText::FromString("�̺�Ʈ �����Ͻ� : 12�� 31��");
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText QuestContent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Compensation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Compensation_Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText EventDue;
};

USTRUCT(BlueprintType)
struct FAbilityDetails : public FTableRowBase
{
	GENERATED_BODY()

public:
	FAbilityDetails() {
		SkillName = FName("Defaults SkillName == SkillDataRowName");
		Descriptsion = FText::FromString("Defaults Descriptsion");
		CoolTime = 3.f;
		CastTime = 1.f;
		Damage = 10.f;
		bIsWideSkill = false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;

	// Skill expression time. If CastTime is 1.f, skill is spawned after 1 second when you clicked skill button
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CastTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Descriptsion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsWideSkill;
};

USTRUCT(BlueprintType)
struct FDeckSaveData {
	GENERATED_BODY()

public:
	FDeckSaveData() {
		BtnNum = -1;
		FSpawnedHeorIndexInDeck.Init(-1, 5);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BtnNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> FSpawnedHeorIndexInDeck;

};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase {
	GENERATED_BODY()

public:
	FWeaponData() {
		WeaponName = FName("Weapon Name");
		Descriptsion = FText::FromString("Descriptsion");
		UpgradeNeedCoin = 10;
		EnforceNum = 0;
		Damage = 10;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Weapon_Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Descriptsion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Test;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpgradeNeedCoin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnforceNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;
};


UCLASS()
class HEROESOFTHECLAY_API ABattleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	

	virtual void BeginPlay() override;

	// JRPG �������� �� ��ȯ ���������� �����ϱ� ������. �Ŀ� ������ ������ ����
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat System")
	TArray<class AHeroCharBase*>TurnOrder; // ������ ������ ���� �迭. 

	// ���� ���۽� ī�޶� ������ ���� ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	class ABattleTopDownCameraActor* CameraActor;


	// ���� ���۽� ��ȯ�Ǵ� �� ĳ���͵��� Ŭ������ ��� �ִ� �迭
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	TArray<class AMyHeroChar*> MyHeroActorArray;

	// ���� ���۽� ��ȯ�Ǵ� �� ĳ���͵��� Ŭ������ ��� �ִ� �迭
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	TArray<class AEnemyHeroChar*> EnemyActorArray;

	// ���� ���� �� ���� �س��� ��� ��ų��. ���� Map - Find �� ���� ��ų�� ������Ű�� ������Ų��. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat System")
	TMap<FName, TSubclassOf<class ASkillActorBase>>All_SkillActor;

	// ============================================== ActorStorage �۾� =========================================================
	// ��� ���͸� ����ִ� �����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save Data")
	TSubclassOf<class ABP_ActorStorage> AllActorStorage;

	// ������ Sapwn�� HeroBP�� �̸��� ���� �迭 (�ε����� �°� Spawn��ġ�� �����ȴ�)
	// �ᱹ �� �迭�� �� ���� ĳ���� ������ŭ ���� ���̴�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<FString> HeroBP_Name;

	// �� ���� �ִ� ��� ĳ���͸� �����ų �迭
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<TSubclassOf<AMyHeroChar>> SavedMyHeroclassArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<TSubclassOf<AMyHeroChar>> SpawnMyHeroClassArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Deck System")
	TSubclassOf<AMyHeroChar> MyHeroClassSetting;
	// SpawnedActorArray�� ������ �����̾�����, �׳� ���� MyHeroActorArray�� �ᵵ �� �� ����. why? ��¥�� ��Ʋ�̵� ���� �̰ɷ� ������ ������ ���� �� �״ϱ�
	// ������ ��Ʋ�ʵ忡�� �۾� ������ ���ӽ��� -> �迭�� ���� ��� ���� ��� ���� -> BattlePosition���� �������� ���ӿ� ��ȯ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<int32> SpawnedHeorIndexInDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<int32> RecommendSpawnHeorIndexInDeck;


public:
	// ���� �ý��� ����
	UFUNCTION(BlueprintCallable)
	void SortingMyHeroArray(TArray<class AMyHeroChar*> MyArray);

	UFUNCTION(BlueprintCallable)
	void SortingEnemyHeroArray(TArray<class AEnemyHeroChar*> EnemyArray);

	UFUNCTION(BlueprintCallable)
	void CheckLastGameOverStatus();

public:
	// �� �ý��� ����
	
	void MakeDeck();
	
	void MakeSpawnArrayFromDeck();

	void GetAllActorsSetting_Deck();

	void SpawnAllHero_Deck();

	UFUNCTION(BlueprintCallable)
	TArray<int32> RecommendAlgorithm_1();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Deck System")
	TSubclassOf<AActor> SpawnAllHeroPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Deck System")
	FTransform PositionTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	int32 SelectedIndexFromDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	struct FDeckSaveData Btn_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	struct FDeckSaveData Btn_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	struct FDeckSaveData Btn_3;

	UFUNCTION(BlueprintCallable)
	FDeckSaveData GetSaveData(int32 BtnNumber);

	UFUNCTION(BlueprintCallable)
	void SetSaveData(int32 BtnNumber, TArray<int32> AlreadySpawnedIndex);
};
