// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPooling/ObjectPoolComponent.h"
#include "ObjectPooling/ObjectPoolActor.h"
#include "SubSystem/ObjectPoolSubSystem.h"



void UObjectPoolComponent::Init()
{
	bIsPoolActive = false;
}

void UObjectPoolComponent::RecycleSelf()
{
	if (UObjectPoolSubSystem* Pool =
		GetWorld()->GetSubsystem<UObjectPoolSubSystem>())
	{
		Pool->RecyclePooledObject(this);
	}
}

void UObjectPoolComponent::OnComponentDestroyed(
	bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(
		bDestroyingHierarchy);
}