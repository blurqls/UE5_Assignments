// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FeatureCharacterController.h"

#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/FeatureCharacterWidget.h"
#include "Components/ProgressBar.h"
#include "GameFramework/SpringArmComponent.h"

AFeatureCharacterController::AFeatureCharacterController()
{
	InputMove = nullptr;
	InputLook = nullptr;
	InputAttack = nullptr;
	InputMappingContext = nullptr;
	
}

void AFeatureCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext,0);
			}
		}
		ShowHUD();
	}
	
}

void AFeatureCharacterController::ShowHUD()
{
	if (HealthWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, HealthWidgetClass, TEXT("Health"));
		
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
