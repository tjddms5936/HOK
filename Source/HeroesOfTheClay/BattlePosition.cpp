// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePosition.h"
#include "HeroCharBase.h"
#include "AIController.h"
#include "MyHeroChar.h"
#include "Components/ArrowComponent.h"


// Sets default values
ABattlePosition::ABattlePosition()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABattlePosition::BeginPlay()
{
	Super::BeginPlay();
	// �������Ʈ���� �������̵�� �������� �Լ� ȣ��
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (Map == FString("BattleLevel")) {
		SpawnUnit();
		UE_LOG(LogTemp, Warning, TEXT("Spawn Success"));
	}
	// SpawnUnit(); // <- �������Ʈ���� �Լ� ������ �� ���. �̷��� ȣ��
	// SpawnUnit_Implementation(); // <- C++������ ������ �̷���.. 
	
	/*if (UnitClass) {
		Unit = UnitClass->GetDefaultObject()->............................
	}*/
}

// Called every frame
void ABattlePosition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattlePosition::SpawnUnit_Deck_Implementation(int32 DeckIndex)
{
	ABattleGameModeBase* GameMode = Cast<ABattleGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		if (GameMode->SavedMyHeroclassArray.IsValidIndex(DeckIndex)) {
			// ���Ӹ�忡 ����� ��ȯ �迭�� ������� �ʴٸ�.
			AMyHeroChar* SpawnHero = GetWorld()->SpawnActor<AMyHeroChar>(GameMode->SavedMyHeroclassArray[DeckIndex], GetActorTransform());
			if (SpawnHero) {
				HeroClass = SpawnHero->GetClass();
				SpawnHero->CharIndex = DeckIndex;
				SpawnHero->PositionIndex = PositionIndex;
				SpawnHero->ActorIndexFromDeck = DeckIndex;
			}
			UE_LOG(LogTemp, Warning, TEXT("Spawn Success"));
			bIsSpawned = true;
		}
		else {
			return;
		}
	}
	return;
}

void ABattlePosition::SpawnUnit_Implementation()
{
	if (HeroClass) {
		// GetWorld()->SpawnActor<AHeroCharBase>(HeroClass, GetActorTransform());
		
		AHeroCharBase* SpawnedHero = GetWorld()->SpawnActor<AHeroCharBase>(HeroClass, GetActorTransform());
		if (SpawnedHero) {
			CharRef = SpawnedHero;
			SpawnedHero->CharIndex = PositionIndex;
			SpawnedHero->SpawnDefaultController();

			AAIController* AICont = Cast<AAIController>(SpawnedHero->GetController());
			if (AICont) {
				SpawnedHero->AIController = AICont;
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Failed. No HeroClass"));
	}
}


