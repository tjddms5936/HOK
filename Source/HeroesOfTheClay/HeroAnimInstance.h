// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HEROESOFTHECLAY_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// AnimInstance에서 BeginPlay || Tick 하고 비슷한 기능
	virtual void NativeInitializeAnimation() override;

	// 블루프린트에서 NativeInitializeAnimation에 연결시켜줌으로써 매 프레임 호출하는거
	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AMyHeroChar* MyHeroCharRef;
};
