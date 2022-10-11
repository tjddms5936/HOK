// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleController.h"
#include "Blueprint/UserWidget.h"

ABattleController::ABattleController() 
{
	// 자동으로 카메라 Active 해주는거 끄기. 우리는 Custom 카메라의 구도를 원함
	bAutoManageActiveCameraTarget = false;
	bShowMouseCursor = true;
}

void ABattleController::BeginPlay()
{
}
