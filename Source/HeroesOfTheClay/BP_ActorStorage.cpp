// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_ActorStorage.h"

// Sets default values
ABP_ActorStorage::ABP_ActorStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABP_ActorStorage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_ActorStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}
