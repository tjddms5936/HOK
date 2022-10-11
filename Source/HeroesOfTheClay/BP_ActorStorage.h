// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_ActorStorage.generated.h"

UCLASS()
class HEROESOFTHECLAY_API ABP_ActorStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABP_ActorStorage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TMap<FString, TSubclassOf<class AMyHeroChar>> BP_AllHeroes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
