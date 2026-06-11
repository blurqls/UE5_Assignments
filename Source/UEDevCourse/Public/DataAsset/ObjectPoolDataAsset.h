// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ObjectPooling/ObjectPoolData.h"
#include "ObjectPoolDataAsset.generated.h"

UCLASS()
class UEDEVCOURSE_API UObjectPoolDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FObjectPoolData> PoolConfigs;
};
