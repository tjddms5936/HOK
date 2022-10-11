// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattlePosition.generated.h"

UCLASS()
class HEROESOFTHECLAY_API ABattlePosition : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattlePosition();

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Val")
	class UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Val")
	TSubclassOf<class AHeroCharBase> HeroClass;

	// 필드 상에 어느 위치에 있는지 HeroChar에게 Index 전달용 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Val")
	int32 PositionIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Val")
	class AHeroCharBase* CharRef;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Val")
	bool bIsEnemyPlacement;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnUnit();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnUnit_Deck(int32 DeckIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Val")
	bool bIsSpawned;
};
