// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPooling/PoolSpawner.h"
#include "SubSystem/ObjectPoolSubSystem.h"



// Called when the game starts or when spawned
void APoolSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&APoolSpawner::SpawnCube,
		3.f,
		true
	);
	
}

void APoolSpawner::SpawnCube()
{
	UObjectPoolSubSystem* Pool =
		GetWorld()->GetSubsystem<UObjectPoolSubSystem>();

	if (!Pool)
	{
		return;
	}

	AActor* Cube =
		Pool->GetPooledActor(TEXT("Cube"));

	if (!Cube)
	{
		return;
	}

	Cube->SetActorLocation(
		FVector(2340.f,1230.f,500.f));

	FTimerHandle Handle;

	GetWorldTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda(
			[Pool, Cube]()
			{
				if (Pool && Cube)
				{
					Pool->RecycleActor(Cube);
				}
			}),
		5.f,
		false);
}


