#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeSignature, float, CurrentHealth, float, MaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEDEVCOURSE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	
public:
	void TakeDamage(float Damage);
	float GetCurrentHealth() const {return CurrentHealth;}
	float GetMaxHealth() const{return MaxHealth;}
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeSignature OnHealthChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;
	
	void BeginPlay() override;
};
