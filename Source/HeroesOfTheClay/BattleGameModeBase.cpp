// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "BattleController.h"
#include "BattleTopDownCameraActor.h"
#include "MyHeroChar.h"
#include "EnemyHeroChar.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BP_ActorStorage.h"

void ABattleGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (Map == FString("DeckLevel")) {
		MakeDeck();
		SpawnAllHero_Deck();
		SelectedIndexFromDeck = -1.f;
		Btn_1.BtnNum = 1;
		Btn_2.BtnNum = 2;
		Btn_3.BtnNum = 3;
		for (int i = 0; i < 5; i++) {
			SpawnedHeorIndexInDeck.Add(-1);
		}
		UE_LOG(LogTemp, Warning, TEXT("Map Name = %s"), *Map);
	}
	else if (Map == FString("BattleLevel")) {
		UE_LOG(LogTemp, Warning, TEXT("Map Name = %s"), *Map);
	}
}

void ABattleGameModeBase::SortingMyHeroArray(TArray<class AMyHeroChar*> MyArray)
{
	if (MyArray.Num() == 0) {
		return;
	}
	TArray<class AMyHeroChar*> newArray = MyArray;
	for (auto HeroActor : MyArray) {
		newArray[HeroActor->CharIndex] = HeroActor;
	}
	MyHeroActorArray = newArray;
}

void ABattleGameModeBase::SortingEnemyHeroArray(TArray<class AEnemyHeroChar*> EnemyArray)
{
	if (EnemyArray.Num() == 0) {
		return;
	}
	TArray<class AEnemyHeroChar*> newArray = EnemyArray;
	for (auto HeroActor : EnemyArray) {
		newArray[HeroActor->CharIndex] = HeroActor;
	}
	EnemyActorArray = newArray;
}

void ABattleGameModeBase::CheckLastGameOverStatus()
{
	if (MyHeroActorArray.Num() > 0) {
		for (auto Actor : MyHeroActorArray) {
			if (Actor->GetHeroMovementStatus() != EHeroMovementStatus::EMS_Dead) {
				Actor->GameTimeOver();
				Actor->SetHeroMovementStatus(EHeroMovementStatus::EMS_Dead);
				if (Actor->GetHeroMovementStatus() == EHeroMovementStatus::EMS_Dead) {
					UE_LOG(LogTemp, Warning, TEXT("Dead Change"));
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("Not Change why??"));
				}
			}
		}
	}
	if (EnemyActorArray.Num() > 0) {
		for (auto Actor : EnemyActorArray) {
			if (Actor->GetHeroMovementStatus() != EHeroMovementStatus::EMS_Dead) {
				Actor->GameTimeOver();
				Actor->SetHeroMovementStatus(EHeroMovementStatus::EMS_Dead);
			}
		}
	}
}

void ABattleGameModeBase::MakeDeck()
{
	if (AllActorStorage) {
		// BP_ActorStorage를 일단 Spawn시켜서 존재시키는 거임. 어쨋든 Actor니까
		ABP_ActorStorage* HeroStorage = GetWorld()->SpawnActor<ABP_ActorStorage>(AllActorStorage);
		if (HeroBP_Name.Num() != 0) {
			// HeroStorage의 Key값이 됨
			for (auto Key : HeroBP_Name) {
				// string배열을 하나씩 다 돌면서(각 String은 HeroStorage의 Key값이 될 것임)
				if (HeroStorage->BP_AllHeroes.Contains(Key)) {
					// 해당 Key값이 있는 경우에만 저장 시킬 것임 (에러 방지)
					// HeroStorage->BP_AllHeroes[Key]는 Class Reference임.
					SavedMyHeroclassArray.Add(HeroStorage->BP_AllHeroes[Key]);
				}
			}
			// for문이 끝난 뒤 SavedMyHeroActorArray의 순서가 의미하는 것은? 
			// 덱 필드 상의 내 덱순서 그대로 의미. 즉, Slot의 Index가 0일 때 그 캐릭터 정보는 배열의 0번째 놈 + Key에 해당하는 Value는 Class임. Actor가 아님.
			MakeSpawnArrayFromDeck();
		}
	}
}

void ABattleGameModeBase::MakeSpawnArrayFromDeck()
{
	if (SavedMyHeroclassArray.Num() != 0) {
		for (int i = 0; i < 5; i++) {
			// 내 덱 배열에서 0~4번째 인덱스까지가 곧 소환 될 배열 순서임.
			SpawnMyHeroClassArray.Add(SavedMyHeroclassArray[i]);
		}
		GetAllActorsSetting_Deck();
	}
}

void ABattleGameModeBase::GetAllActorsSetting_Deck()
{
	TArray<AActor*> Array;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), MyHeroClassSetting, Array);
	for (auto Actor : Array) {
		AMyHeroChar* MyHero = Cast<AMyHeroChar>(Actor);
		if (MyHero->IsJustSpawnedForDeck != true) {
			MyHeroActorArray.Add(MyHero);
		}
	}
}

void ABattleGameModeBase::SpawnAllHero_Deck()
{
	if (SavedMyHeroclassArray.Num() != 0) {
		// 덱에 저장된 Hero Class가 존재하면 일단 모두 스폰시킴. why? Get Actor Of Class를 위하여 짠!
		// 겹치는 거 스폰시킬 필요없는데, 나중에 겹치면 뭐 캐릭 레벨업으로 간다던가 해버리자. 그니깐 덱에는 겹치는 HeroClass가 없는거지 쿠키런 킹덤 처럼 
		if (SpawnAllHeroPosition) {
			AActor* PositionActor = Cast<AActor>(UGameplayStatics::GetActorOfClass(GetWorld(), SpawnAllHeroPosition));
			if (PositionActor) {
				PositionTransform = PositionActor->GetTransform();
				for (auto HeroClass : SavedMyHeroclassArray) {
					AMyHeroChar* SpawnHero = GetWorld()->SpawnActor<AMyHeroChar>(HeroClass, PositionTransform);
					if (SpawnHero) {
						SpawnHero->IsJustSpawnedForDeck = true;
						UE_LOG(LogTemp, Warning, TEXT("Just Spawn Actor Success!!!"));
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("Just Spawn Actor Failed!!!"));
					}
				}
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("PositionActor Casting Failed"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SpawnAllHeroPosition N O!!!"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SavedMyHeroclassArray.Num() == 0"));
	}
}

TArray<int32> ABattleGameModeBase::RecommendAlgorithm_1()
{
	// 가장 높은 전투력 조합 찾아줌 
	// RecommendSpawnHeorIndexInDeck
	if (SavedMyHeroclassArray.Num() >= 5) {
		TArray<int32> CombatPower;
		// 최소 5개 이상은 되어야 함 그 이하도 조합 찾아주긴 하면 좋긴한데.. 아니다, 5개 이하면 그냥 캐릭터가 5개 이하라 추전조합 할 수 없다고 하자 
		for (int i = 0; i < 5; i++) {
			AMyHeroChar* MyHero = Cast<AMyHeroChar>(SavedMyHeroclassArray[i]->GetDefaultObject());
			CombatPower.Add(MyHero->OffensivePower); // 일단 0~4번째 놈들의 공격력을 각각 저장해줌. 각 Index를 돌면서 더 큰놈이 나오면 그놈이랑 교환해주면 됨. 순서대로 크진 않지만 전체적으로는 가장 큰 조합일것임
			RecommendSpawnHeorIndexInDeck.Add(i); // 일단 0~4번째 인덱스를 저장해놓음
		}
		if (SavedMyHeroclassArray.Num() >= 6) {
			// 애초에 5개밖에 없으면 추전 조합이 의미가 없으니까 최소 6개 이상은 되어야 함 
			for (int i = 5; i < SavedMyHeroclassArray.Num(); i++) {
				AMyHeroChar* MyHero2 = Cast<AMyHeroChar>(SavedMyHeroclassArray[i]->GetDefaultObject());
				int32 CPower = MyHero2->OffensivePower;
				// MyHero2랑 기존에 저장된 CombatPower랑 비교해서 어떤놈보다 크다면 바꿔줌 
				for (int j = 0; j < 5; j++) {
					if (CPower > CombatPower[j]) {
						CombatPower[j] = CPower;
						RecommendSpawnHeorIndexInDeck[j] = i;
						break;
					}
				}
			}
		}
	}
	// RecommendSpawnHeorIndexInDeck 에는 가장 전투력이 쎈 5놈의 Deck에서 Index 값을 가지고 있음. 
	return RecommendSpawnHeorIndexInDeck;
}

FDeckSaveData ABattleGameModeBase::GetSaveData(int32 BtnNumber)
{
	switch (BtnNumber)
	{
	case 1:
		UE_LOG(LogTemp, Warning, TEXT("return Struct Sucess"));
		return Btn_1;
	case 2:
		return Btn_2;
	case 3:
		return Btn_3;
	default:
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("return Struct Fail!!!!"));
	return FDeckSaveData();
}

void ABattleGameModeBase::SetSaveData(int32 BtnNumber, TArray<int32> AlreadySpawnedIndex)
{
	switch (BtnNumber)
	{
	case 1:
		Btn_1.FSpawnedHeorIndexInDeck = AlreadySpawnedIndex;
		break;
	case 2:
		Btn_2.FSpawnedHeorIndexInDeck = AlreadySpawnedIndex;
		break;
	case 3:
		Btn_3.FSpawnedHeorIndexInDeck = AlreadySpawnedIndex;
	default:
		break;
	}
	return;
}

