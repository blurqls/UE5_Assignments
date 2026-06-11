// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FeatureAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndSignature);

/**
 * 
 */
UCLASS()
class UEDEVCOURSE_API UFeatureAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	FOnAttackEndSignature OnEndAttack;
	
	
	
	UFUNCTION()
	void AnimNotify_AttackEnd();
	
	
};
