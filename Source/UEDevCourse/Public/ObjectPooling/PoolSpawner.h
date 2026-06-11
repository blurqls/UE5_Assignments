// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolSpawner.generated.h"

UCLASS()
class UEDEVCOURSE_API APoolSpawner : public AActor
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	FTimerHandle SpawnTimerHandle;

	void SpawnCube();

};
