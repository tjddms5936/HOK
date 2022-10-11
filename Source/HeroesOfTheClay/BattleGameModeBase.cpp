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
		// BP_ActorStorage�� �ϴ� Spawn���Ѽ� �����Ű�� ����. ��¶�� Actor�ϱ�
		ABP_ActorStorage* HeroStorage = GetWorld()->SpawnActor<ABP_ActorStorage>(AllActorStorage);
		if (HeroBP_Name.Num() != 0) {
			// HeroStorage�� Key���� ��
			for (auto Key : HeroBP_Name) {
				// string�迭�� �ϳ��� �� ���鼭(�� String�� HeroStorage�� Key���� �� ����)
				if (HeroStorage->BP_AllHeroes.Contains(Key)) {
					// �ش� Key���� �ִ� ��쿡�� ���� ��ų ���� (���� ����)
					// HeroStorage->BP_AllHeroes[Key]�� Class Reference��.
					SavedMyHeroclassArray.Add(HeroStorage->BP_AllHeroes[Key]);
				}
			}
			// for���� ���� �� SavedMyHeroActorArray�� ������ �ǹ��ϴ� ����? 
			// �� �ʵ� ���� �� ������ �״�� �ǹ�. ��, Slot�� Index�� 0�� �� �� ĳ���� ������ �迭�� 0��° �� + Key�� �ش��ϴ� Value�� Class��. Actor�� �ƴ�.
			MakeSpawnArrayFromDeck();
		}
	}
}

void ABattleGameModeBase::MakeSpawnArrayFromDeck()
{
	if (SavedMyHeroclassArray.Num() != 0) {
		for (int i = 0; i < 5; i++) {
			// �� �� �迭���� 0~4��° �ε��������� �� ��ȯ �� �迭 ������.
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
		// ���� ����� Hero Class�� �����ϸ� �ϴ� ��� ������Ŵ. why? Get Actor Of Class�� ���Ͽ� §!
		// ��ġ�� �� ������ų �ʿ���µ�, ���߿� ��ġ�� �� ĳ�� ���������� ���ٴ��� �ع�����. �״ϱ� ������ ��ġ�� HeroClass�� ���°��� ��Ű�� ŷ�� ó�� 
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
	// ���� ���� ������ ���� ã���� 
	// RecommendSpawnHeorIndexInDeck
	if (SavedMyHeroclassArray.Num() >= 5) {
		TArray<int32> CombatPower;
		// �ּ� 5�� �̻��� �Ǿ�� �� �� ���ϵ� ���� ã���ֱ� �ϸ� �����ѵ�.. �ƴϴ�, 5�� ���ϸ� �׳� ĳ���Ͱ� 5�� ���϶� �������� �� �� ���ٰ� ���� 
		for (int i = 0; i < 5; i++) {
			AMyHeroChar* MyHero = Cast<AMyHeroChar>(SavedMyHeroclassArray[i]->GetDefaultObject());
			CombatPower.Add(MyHero->OffensivePower); // �ϴ� 0~4��° ����� ���ݷ��� ���� ��������. �� Index�� ���鼭 �� ū���� ������ �׳��̶� ��ȯ���ָ� ��. ������� ũ�� ������ ��ü�����δ� ���� ū �����ϰ���
			RecommendSpawnHeorIndexInDeck.Add(i); // �ϴ� 0~4��° �ε����� �����س���
		}
		if (SavedMyHeroclassArray.Num() >= 6) {
			// ���ʿ� 5���ۿ� ������ ���� ������ �ǹ̰� �����ϱ� �ּ� 6�� �̻��� �Ǿ�� �� 
			for (int i = 5; i < SavedMyHeroclassArray.Num(); i++) {
				AMyHeroChar* MyHero2 = Cast<AMyHeroChar>(SavedMyHeroclassArray[i]->GetDefaultObject());
				int32 CPower = MyHero2->OffensivePower;
				// MyHero2�� ������ ����� CombatPower�� ���ؼ� ��𺸴� ũ�ٸ� �ٲ��� 
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
	// RecommendSpawnHeorIndexInDeck ���� ���� �������� �� 5���� Deck���� Index ���� ������ ����. 
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

