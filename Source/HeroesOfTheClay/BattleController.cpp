// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleController.h"
#include "Blueprint/UserWidget.h"

ABattleController::ABattleController() 
{
	// �ڵ����� ī�޶� Active ���ִ°� ����. �츮�� Custom ī�޶��� ������ ����
	bAutoManageActiveCameraTarget = false;
	bShowMouseCursor = true;
}

void ABattleController::BeginPlay()
{
}
