// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FeatureCharacter.generated.h"

struct FInputActionValue;
class UStatComponent;

UCLASS()
class UEDEVCOURSE_API AFeatureCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFeatureCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Character Movement
	void CharacterMove(const FInputActionValue& InputAction);
	
	//Character Look
	void CharacterLook(const FInputActionValue& InputAction);
	
	//Character Attack
	void CharacterAttack(const FInputActionValue& InputAction);
	
	//Delegate Bind Health
	UFUNCTION()
	void HealthLog(float CurrentHealth, float MaxHealth);
	
	//Delegate Bind Death
	UFUNCTION()
	void HandleDeath();
	
	UFUNCTION()
	UStatComponent* GetStatComponent() {return StatComp;}

	UFUNCTION()
	void AttackEnd();

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent> StatComp; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<class UFeatureAnimInstance> AnimInstance;
	
	UPROPERTY(VisibleAnywhere)
	bool IsDead;
	bool IsAttacking;
};
