// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/FeatureAnimInstance.h"

void UFeatureAnimInstance::AnimNotify_AttackEnd()
{
	OnEndAttack.Broadcast();
}
