// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTopDownCameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABattleTopDownCameraActor::ABattleTopDownCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 1200.f;
	SpringArmComponent->bUsePawnControlRotation = false; // 컨트롤러에 의해 카메라가 회전되는 것을 막기 위함
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void ABattleTopDownCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattleTopDownCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

