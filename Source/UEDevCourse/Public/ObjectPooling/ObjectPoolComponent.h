// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEDEVCOURSE_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	void Init();
	
	UFUNCTION(BlueprintCallable)
	void RecycleSelf();
	
	UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly)
	bool bIsPoolActive;
	
private:
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

		
};
