#include "SubSystem/ObjectPoolSubSystem.h"

#include "DataAsset/ObjectPoolDataAsset.h"
#include "ObjectPooling/ObjectPoolComponent.h"

void UObjectPoolSubSystem::Initialize(
	FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (!GetWorld()->IsGameWorld()) return;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Pool Subsystem Created")
	);

	UObjectPoolDataAsset* Data =
		LoadObject<UObjectPoolDataAsset>(
			nullptr,
			TEXT("/Game/DataAssets/DA_ObjectPool.DA_ObjectPool")
		);

	if (!Data)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Failed To Load DA_ObjectPool")
		);

		return;
	}

	ListofObjects = Data->PoolConfigs;

	BuildPools();
}

void UObjectPoolSubSystem::Deinitialize()
{
	Pools.Empty();
	ListofObjects.Empty();

	Super::Deinitialize();
}

void UObjectPoolSubSystem::BuildPools()
{
	FActorSpawnParameters SpawnParams;

	for (int32 PoolIndex = 0;
		PoolIndex < ListofObjects.Num();
		PoolIndex++)
	{
		FSingleObjectPool CurrentPool;

		SpawnParams.Name =
			FName(*ListofObjects[PoolIndex].ActorName);

		SpawnParams.NameMode =
			FActorSpawnParameters::
			ESpawnActorNameMode::Requested;

		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::
			AlwaysSpawn;

		for (int32 i = 0;
			i < ListofObjects[PoolIndex].SpawnActorNum;
			i++)
		{
			AActor* SpawnedActor =
				GetWorld()->SpawnActor(
					ListofObjects[PoolIndex].WhichActorSpawn,
					&FVector::ZeroVector,
					&FRotator::ZeroRotator,
					SpawnParams);

			if (!SpawnedActor)
			{
				continue;
			}

			SpawnedActor->SetActorLabel(
				SpawnedActor->GetName());

			UObjectPoolComponent* PoolComp =
				NewObject<UObjectPoolComponent>(
					SpawnedActor);

			PoolComp->RegisterComponent();

			SpawnedActor->AddInstanceComponent(
				PoolComp);

			PoolComp->Init();

			CurrentPool.PooledObjects.Add(
				PoolComp);

			SpawnedActor->SetActorHiddenInGame(
				true);

			SpawnedActor->SetActorEnableCollision(
				false);

			SpawnedActor->SetActorTickEnabled(
				false);

			SpawnedActor->SetActorLocation(
				SaveLocation);
		}

		Pools.Add(CurrentPool);
	}
}

void UObjectPoolSubSystem::Broadcast_PoolerCleanup()
{
	OnPoolerCleanup.Broadcast();
}

AActor* UObjectPoolSubSystem::GetPooledActor(
	FString Name)
{
	int32 PoolCount = Pools.Num();

	int32 CurrentPoolIndex = -1;

	for (int32 i = 0; i < PoolCount; i++)
	{
		if (ListofObjects[i].ActorName == Name)
		{
			CurrentPoolIndex = i;
			break;
		}
	}

	if (CurrentPoolIndex == -1)
	{
		return nullptr;
	}

	int32 PooledObjectCount =
		Pools[CurrentPoolIndex]
		.PooledObjects.Num();

	int32 FirstAvailable = -1;

	for (int32 i = 0;
		i < PooledObjectCount;
		i++)
	{
		if (Pools[CurrentPoolIndex]
			.PooledObjects[i] != nullptr)
		{
			if (!Pools[CurrentPoolIndex]
				.PooledObjects[i]
				->bIsPoolActive)
			{
				FirstAvailable = i;
				break;
			}
		}
		else
		{
			RegenItem(
				CurrentPoolIndex,
				i);

			FirstAvailable = i;

			break;
		}
	}

	if (FirstAvailable >= 0)
	{
		UObjectPoolComponent* ToReturn =
			Pools[CurrentPoolIndex]
			.PooledObjects[FirstAvailable];

		ToReturn->bIsPoolActive = true;

		OnPoolerCleanup.AddUniqueDynamic(
			ToReturn,
			&UObjectPoolComponent::RecycleSelf);

		AActor* ToReturnActor =
			ToReturn->GetOwner();

		ToReturnActor->SetActorHiddenInGame(
			false);

		ToReturnActor->SetActorEnableCollision(
			true);

		ToReturnActor->SetActorTickEnabled(
			true);
		
		UPrimitiveComponent* Primitive =
	Cast<UPrimitiveComponent>(
		ToReturnActor->GetRootComponent());

		if (Primitive)
		{
			Primitive->SetSimulatePhysics(true);
		}

		return ToReturnActor;
	}

	if (!ListofObjects[CurrentPoolIndex]
		.CanGrow)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;

	SpawnParams.Name =
		FName(
			*ListofObjects[CurrentPoolIndex]
			.ActorName);

	SpawnParams.NameMode =
		FActorSpawnParameters::
		ESpawnActorNameMode::Requested;

	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::
		AlwaysSpawn;

	AActor* SpawnedActor =
		GetWorld()->SpawnActor(
			ListofObjects[CurrentPoolIndex]
			.WhichActorSpawn,
			&FVector::ZeroVector,
			&FRotator::ZeroRotator,
			SpawnParams);

	SpawnedActor->SetActorLabel(
		SpawnedActor->GetName());

	UObjectPoolComponent* PoolComp =
		NewObject<UObjectPoolComponent>(
			SpawnedActor);

	PoolComp->RegisterComponent();

	SpawnedActor->AddInstanceComponent(
		PoolComp);

	PoolComp->Init();

	Pools[CurrentPoolIndex]
		.PooledObjects.Add(PoolComp);

	PoolComp->bIsPoolActive = true;

	OnPoolerCleanup.AddUniqueDynamic(
		PoolComp,
		&UObjectPoolComponent::RecycleSelf);

	return SpawnedActor;
}

void UObjectPoolSubSystem::RecyclePooledObject(
	UObjectPoolComponent* PoolCompRef)
{
	OnPoolerCleanup.RemoveDynamic(
		PoolCompRef,
		&UObjectPoolComponent::RecycleSelf);

	PoolCompRef->bIsPoolActive = false;

	AActor* ReturningActor =
		PoolCompRef->GetOwner();

	ReturningActor->SetActorHiddenInGame(
		true);

	ReturningActor->SetActorEnableCollision(
		false);

	ReturningActor->SetActorTickEnabled(
		false);

	ReturningActor->SetActorLocation(
		SaveLocation);
	
	UPrimitiveComponent* Primitive =
	Cast<UPrimitiveComponent>(
		ReturningActor->GetRootComponent());

	if (Primitive)
	{
		Primitive->SetSimulatePhysics(false);
		Primitive->SetPhysicsLinearVelocity(FVector::ZeroVector);
		Primitive->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}
}

void UObjectPoolSubSystem::RecycleActor(
	AActor* PooledActor)
{
	if (UObjectPoolComponent* PoolCompRef =
		Cast<UObjectPoolComponent>(
			PooledActor->GetComponentByClass(
				UObjectPoolComponent::StaticClass())))
	{
		RecyclePooledObject(
			PoolCompRef);
	}
}

void UObjectPoolSubSystem::RegenItem(
	int32 PoolIndex,
	int32 PositionIndex)
{
	FActorSpawnParameters SpawnParams;

	SpawnParams.Name =
		FName(
			*ListofObjects[PoolIndex]
			.ActorName);

	SpawnParams.NameMode =
		FActorSpawnParameters::
		ESpawnActorNameMode::Requested;

	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::
		AlwaysSpawn;

	AActor* SpawnedActor =
		GetWorld()->SpawnActor(
			ListofObjects[PoolIndex]
			.WhichActorSpawn,
			&FVector::ZeroVector,
			&FRotator::ZeroRotator,
			SpawnParams);

	SpawnedActor->SetActorLabel(
		SpawnedActor->GetName());

	UObjectPoolComponent* PoolComp =
		NewObject<UObjectPoolComponent>(
			SpawnedActor);

	PoolComp->RegisterComponent();

	SpawnedActor->AddInstanceComponent(
		PoolComp);

	PoolComp->Init();

	Pools[PoolIndex]
		.PooledObjects.Insert(
			PoolComp,
			PositionIndex);

	SpawnedActor->SetActorHiddenInGame(
		true);

	SpawnedActor->SetActorEnableCollision(
		false);

	SpawnedActor->SetActorTickEnabled(
		false);

	SpawnedActor->SetActorLocation(
		SaveLocation);
}