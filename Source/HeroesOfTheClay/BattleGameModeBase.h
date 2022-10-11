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
		EventDue = FText::FromString("이벤트 종료일시 : 12월 31일");
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

	// JRPG 형식으로 턴 전환 전투형식을 구현하기 위함임. 후에 삭제나 수정할 것임
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat System")
	TArray<class AHeroCharBase*>TurnOrder; // 전투의 순서를 위한 배열. 

	// 게임 시작시 카메라 구도를 위한 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	class ABattleTopDownCameraActor* CameraActor;


	// 게임 시작시 소환되는 내 캐릭터들의 클래스를 담고 있는 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	TArray<class AMyHeroChar*> MyHeroActorArray;

	// 게임 시작시 소환되는 적 캐릭터들의 클래스를 담고 있는 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	TArray<class AEnemyHeroChar*> EnemyActorArray;

	// 게임 제작 시 구현 해놓은 모든 스킬들. 이후 Map - Find 를 통해 스킬을 연동시키고 스폰시킨다. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat System")
	TMap<FName, TSubclassOf<class ASkillActorBase>>All_SkillActor;

	// ============================================== ActorStorage 작업 =========================================================
	// 모든 액터를 담고있는 저장소
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save Data")
	TSubclassOf<class ABP_ActorStorage> AllActorStorage;

	// 덱에서 Sapwn될 HeroBP의 이름을 담은 배열 (인덱스에 맞게 Spawn위치가 결정된다)
	// 결국 이 배열은 내 덱의 캐릭터 갯수만큼 있을 것이다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<FString> HeroBP_Name;

	// 내 덱에 있는 모든 캐릭터를 저장시킬 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<TSubclassOf<AMyHeroChar>> SavedMyHeroclassArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<TSubclassOf<AMyHeroChar>> SpawnMyHeroClassArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Deck System")
	TSubclassOf<AMyHeroChar> MyHeroClassSetting;
	// SpawnedActorArray를 선언할 예정이었으나, 그냥 위의 MyHeroActorArray로 써도 될 것 같음. why? 어짜피 배틀이든 뭐든 이걸로 정해진 놈으로 쓰게 될 테니까
	// 기존의 배틀필드에서 작업 순서가 게임시작 -> 배열에 액터 담기 에서 담긴 액터 -> BattlePosition에서 연동시켜 게임에 소환

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<int32> SpawnedHeorIndexInDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck System")
	TArray<int32> RecommendSpawnHeorIndexInDeck;


public:
	// 전투 시스템 관련
	UFUNCTION(BlueprintCallable)
	void SortingMyHeroArray(TArray<class AMyHeroChar*> MyArray);

	UFUNCTION(BlueprintCallable)
	void SortingEnemyHeroArray(TArray<class AEnemyHeroChar*> EnemyArray);

	UFUNCTION(BlueprintCallable)
	void CheckLastGameOverStatus();

public:
	// 덱 시스템 관련
	
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
