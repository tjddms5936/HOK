// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "HOKController.generated.h"

/**
 * 
 */


UCLASS()
class HEROESOFTHECLAY_API AHOKController : public APlayerController
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BP_ActorStorage")
	TSubclassOf<class ABP_ActorStorage> BP_ActorStorage;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContorlMovingScreen")
	bool IsMovingScreen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
