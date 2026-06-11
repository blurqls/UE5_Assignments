// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolData.generated.h"

USTRUCT(BlueprintType)
struct FObjectPoolData
{
	GENERATED_BODY()
	
	FObjectPoolData()
	{
		WhichActorSpawn = nullptr;
		SpawnActorNum = 0;
		CanGrow = false;
		ActorName = "";
	}
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor> WhichActorSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnActorNum;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool CanGrow;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ActorName;
	
};
