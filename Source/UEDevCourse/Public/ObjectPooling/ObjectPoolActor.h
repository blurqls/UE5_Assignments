// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPoolActor.generated.h"

/*DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPoolerCleanupSignature);*/

/*USTRUCT(BlueprintType)
struct FSingleObjectPoolActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<class UObjectPoolComponent>> PooledObjects;
	
};*/
UCLASS()
class UEDEVCOURSE_API AObjectPoolActor : public AActor
{
	GENERATED_BODY()

/*
protected:
	virtual void BeginPlay() override;
	
public:
	
	UPROPERTY()
	FPoolerCleanupSignature OnPoolerCleanup;
	
	UFUNCTION(BlueprintCallable)
	void Broadcast_PoolerCleanup();

	UFUNCTION(BlueprintCallable)
	AActor* GetPooledActor(FString Name);

	UFUNCTION(BlueprintCallable)
	void RecyclePooledObject(class UObjectPoolComponent* PoolCompRef);

	UFUNCTION(BlueprintCallable)
	void RecycleActor(AActor* PooledActor);
	
	//레시피
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<struct FObjectPoolData> ListofObjects;

protected:

	//만들어 둔 액터에 붙어있는 컴포넌트를 모아둔 장소
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FSingleObjectPool> Pools;

private:
	void RegenItem(int32 PoolIndex, int32 PositionIndex);
	*/

};
