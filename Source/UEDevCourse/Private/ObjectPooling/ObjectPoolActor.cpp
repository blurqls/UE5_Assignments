// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPooling/ObjectPoolActor.h"
#include "ObjectPooling/ObjectPoolData.h"
#include "ObjectPooling/ObjectPoolComponent.h"

/*
void AObjectPoolActor::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	
	for (int PoolIndex = 0; PoolIndex < ListofObjects.Num(); PoolIndex++)
	{
		FSingleObjectPool CurrentPoolIndex;
		SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *ListofObjects[PoolIndex].ActorName));
		SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		for (int i =0; i< ListofObjects[PoolIndex].SpawnActorNum; i++)
		{
			AActor* SpawnedActor = GetWorld()->SpawnActor(ListofObjects[PoolIndex].WhichActorSpawn, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
			SpawnedActor->SetActorLabel(SpawnedActor->GetName());
			UObjectPoolComponent* PoolComp = NewObject<UObjectPoolComponent>(SpawnedActor);
			PoolComp->RegisterComponent();
			SpawnedActor->AddInstanceComponent(PoolComp);
			PoolComp->Init();
			CurrentPoolIndex.PooledObjects.Add(PoolComp);
			SpawnedActor->SetActorHiddenInGame(true);
			SpawnedActor->SetActorEnableCollision(false);
			SpawnedActor->SetActorTickEnabled(false);
			SpawnedActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			
		}
		Pools.Add(CurrentPoolIndex);
	}
	
}

void AObjectPoolActor::Broadcast_PoolerCleanup()
{
	OnPoolerCleanup.Broadcast();
}

AActor* AObjectPoolActor::GetPooledActor(FString Name)
{
	int32 PoolCount = Pools.Num();
	int32 CurrentPoolIndex = -1;
	
	for (int32 i = 0; i< PoolCount; i++)
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
	
	int32 PooledObjectCount = Pools[CurrentPoolIndex].PooledObjects.Num();
	int32 FirstAvaliable = -1;
	//순회하기
	//우리가 찾은 인덱스 안에서 Active중이지 않은 첫번째 애를 찾기
	for (int32 i = 0; i < PooledObjectCount; i++)
	{
		if (Pools[CurrentPoolIndex].PooledObjects[i] != nullptr)
		{
			if (!Pools[CurrentPoolIndex].PooledObjects[i]->bIsPoolActive)
			{
				FirstAvaliable = i;
				break;
			}
		}
		else //비어있다면 황급히 그 인덱스에 만들기
		{
			RegenItem(CurrentPoolIndex, i);
			FirstAvaliable = i;
			break;
		}
	}



	if (FirstAvaliable >= 0)
	{
		//만들어진 오브젝트의 컴포넌트를 끌고와서 사용중으로 만들어줌
		UObjectPoolComponent* ToReturn = Pools[CurrentPoolIndex].PooledObjects[FirstAvaliable];
		ToReturn->bIsPoolActive = true;

		//회수 예약
		OnPoolerCleanup.AddUniqueDynamic(ToReturn, &UObjectPoolComponent::RecycleSelf);

		//숨겨놨던걸 다시 보이게 해줌
		AActor* ToReturnActor = ToReturn->GetOwner();
		ToReturnActor->SetActorHiddenInGame(false);
		ToReturnActor->SetActorEnableCollision(true);
		ToReturnActor->SetActorTickEnabled(true);

		//nullptr을 넣어주면 부모를 끊고 월드 최상단으로 가겠다는 뜻.
		ToReturnActor->AttachToActor(nullptr, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		
		return ToReturnActor;
	}
	
	//GLOW가 켜져있다면 새로 만들어서 주는 로직 수행
	if (!ListofObjects[CurrentPoolIndex].CanGrow) { return nullptr; }

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *ListofObjects[CurrentPoolIndex].ActorName));
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AActor* SpawnedActor = GetWorld()->SpawnActor(ListofObjects[CurrentPoolIndex].WhichActorSpawn,&FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
	

	SpawnedActor->SetActorLabel(SpawnedActor->GetName());
	UObjectPoolComponent* PoolComp = NewObject<UObjectPoolComponent>(SpawnedActor);
	PoolComp->RegisterComponent();
	SpawnedActor->AddInstanceComponent(PoolComp);
	PoolComp->Init();
	Pools[CurrentPoolIndex].PooledObjects.Add(PoolComp);
	PoolComp->bIsPoolActive = true;
	OnPoolerCleanup.AddUniqueDynamic(PoolComp, &UObjectPoolComponent::RecycleSelf);
	return SpawnedActor;

}

void AObjectPoolActor::RecyclePooledObject(class UObjectPoolComponent* PoolCompRef)
{
	OnPoolerCleanup.RemoveDynamic(PoolCompRef, &UObjectPoolComponent::RecycleSelf);

	PoolCompRef->bIsPoolActive = false;
	AActor* ReturningActor = PoolCompRef->GetOwner();
	ReturningActor->SetActorHiddenInGame(true);
	ReturningActor->SetActorEnableCollision(false);
	ReturningActor->SetActorTickEnabled(false);

	ReturningActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AObjectPoolActor::RecycleActor(AActor* PooledActor)
{
	if (UObjectPoolComponent* PoolCompRef = Cast<UObjectPoolComponent>(PooledActor->GetComponentByClass(UObjectPoolComponent::StaticClass())))
	{
		RecyclePooledObject(PoolCompRef);
	}
}

void AObjectPoolActor::RegenItem(int PoolIndex, int PositionIndex)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *ListofObjects[PoolIndex].ActorName));
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	AActor* SpawnedActor = GetWorld()->SpawnActor(ListofObjects[PoolIndex].WhichActorSpawn,&FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
	
	//외부 아웃라이너 이름 바꾸기
	SpawnedActor->SetActorLabel(SpawnedActor->GetName());

	UObjectPoolComponent* PoolComp = NewObject<UObjectPoolComponent>(SpawnedActor);
	PoolComp->RegisterComponent();
	SpawnedActor->AddInstanceComponent(PoolComp);
	PoolComp->Init();

	Pools[PoolIndex].PooledObjects.Insert(PoolComp, PositionIndex);
	SpawnedActor->SetActorHiddenInGame(true);
	SpawnedActor->SetActorEnableCollision(false);
	SpawnedActor->SetActorTickEnabled(false);
	SpawnedActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
}
*/

