#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TickDamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEDEVCOURSE_API UTickDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTickDamageComponent();

protected:
	virtual auto BeginPlay() -> void override;
	
	// variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageVariable")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageVariable")
	float TickTime;
	


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Returns the Damage value
	float GetDamage() {return Damage;}
	
	// Checking if it is in Touch
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult); 
	
	
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	

	
		
};
