#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "ObjectPooling/ObjectPoolData.h"
#include "DataAsset/ObjectPoolDataAsset.h"

#include "ObjectPoolSubSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FPoolerCleanupSignature);

USTRUCT(BlueprintType)
struct FSingleObjectPool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<class UObjectPoolComponent>>
		PooledObjects;
};

UCLASS()
class UEDEVCOURSE_API UObjectPoolSubSystem
	: public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(
		FSubsystemCollectionBase& Collection)
	override;

	virtual void Deinitialize() override;

public:

	UPROPERTY()
	FPoolerCleanupSignature OnPoolerCleanup;

	void Broadcast_PoolerCleanup();

	AActor* GetPooledActor(FString Name);

	void RecyclePooledObject(
		class UObjectPoolComponent* PoolCompRef);

	void RecycleActor(AActor* PooledActor);

private:

	UPROPERTY()
	TArray<FObjectPoolData> ListofObjects;

	UPROPERTY()
	TArray<FSingleObjectPool> Pools;
	
	UPROPERTY()
	FVector SaveLocation = FVector(0.f, 0.f, -1000.f);

private:

	void RegenItem(
		int32 PoolIndex,
		int32 PositionIndex);

	void BuildPools();
};