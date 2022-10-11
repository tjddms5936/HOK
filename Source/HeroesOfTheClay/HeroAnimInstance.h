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
	// AnimInstance���� BeginPlay || Tick �ϰ� ����� ���
	virtual void NativeInitializeAnimation() override;

	// �������Ʈ���� NativeInitializeAnimation�� ������������ν� �� ������ ȣ���ϴ°�
	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AMyHeroChar* MyHeroCharRef;
};
