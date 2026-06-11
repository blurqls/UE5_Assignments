// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FeatureCharacterController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class UEDEVCOURSE_API AFeatureCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFeatureCharacterController();
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere)
	UInputAction* InputMove;
	UPROPERTY(EditAnywhere)
	UInputAction* InputLook;
	UPROPERTY(EditAnywhere)
	UInputAction* InputAttack;
	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* InputMappingContext;
	
	UFUNCTION(BlueprintCallable, Category= "UI")
	void ShowHUD();
protected:
	//BP에서 할당할 위젯 BP 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthWidgetClass;
	
	UPROPERTY()
	UUserWidget* CurrentWidget;
	
};
